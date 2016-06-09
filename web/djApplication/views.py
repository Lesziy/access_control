#-*- coding: utf-8 -*-
from django.shortcuts import render
from django.template import RequestContext
from django.shortcuts import render_to_response
from django.http import HttpResponse, JsonResponse
from calendar import Calendar
from datetime import date
from socket import *
import json
import logging
import os
import sha3
from django.conf import settings
months = ['Styczeń', 'Luty', 'Marzec', 'Kwiecień', 'Maj', 'Czerwiec', 'Lipiec', 'Sierpień', 'Wrzesień', 'Październik', 'Listopad', 'Grudzień']
dayNames = ['Pon', 'Wt', 'Śr', 'Czw','Pt','Sob','Nie']
logging.basicConfig(filename = settings.BASE_DIR + '/logs/log.log',level = logging.DEBUG, format = '%(asctime)s %(levelname)s: %(message)s',datefmt = '%m/%d/%Y %H:%M:%S')
sessions = {}
socketPort = 3490
socketAddress = 'localhost'
#GŁÓWNY KONTROLER#
def index(request):
   # 'request' contains info about user
   #context = RequestContext(request)

   if request.method == 'POST':
      if "loginSubmit" in request.POST:
         loginSite(request)
      if "logoutSubmit" in request.POST:
         logout(request)
         return render(request, 'reservationSite.html')
      if "dir" in request.POST:
         return jsonCalendar(request)
      if "reserve" in request.POST:
         return HttpResponse(processReservation(request))
      if "resign" in request.POST:
         return HttpResponse(processResignation(request))
      if "calendar" in request.POST:
         return reservedHours(request)
   #hold session
   
   if "username" in request.session:
      return render(request, 'calendar.html',calendarSite(date.today().year, date.today().month))
   #basic login template 
   return render(request, 'reservationSite.html')

#
def processResignation(request):
   resignation = json.loads(request.POST['resign'])
   resignation['month'] = resignation['month'].encode("utf-8")
   time = countReservations(resignation['time'])
   if(socketResignation(sessions[request.session.session_key], time, resignation, request.session["username"])):
      return "rezygnacja zaakceptowana"
   return "rezygnacja odrzucona"
def processReservation(request):
   reservation = json.loads(request.POST['reserve'])
   ##convert request to utf-8
   reservation['month'] = reservation['month'].encode("utf-8")
   time = countReservations(reservation['time'])
   if(socketReservation(sessions[request.session.session_key], time, reservation, request.session["username"])):
      return "rezerwacja/e zaakceptowana"
   return "jedna lub więcej rezerwacji odrzucona"
def socketReservation(sessionSocket, reservations, reservationJSON, user):
   duration = 0
   returned = True
   #sprawdzenie danych
   if((not reservationJSON['month'] in months) or (not int(reservationJSON['year']) in range(1900, 2500)) or (not int(reservationJSON['day']) in range(1,31))):
      logging.error('malformed reservation request')
      return False
   for exclusiveReservation in reservations:
      if((not exclusiveReservation[-1] in range(0,23)) or (not exclusiveReservation[0] in range(0,23) or exclusiveReservation[0] > exclusiveReservation[-1])):
         returned = False
         continue
      duration = exclusiveReservation[-1] - exclusiveReservation[0] +1
      month = months.index(reservationJSON['month'])+1
      if(month<10):
         month = '0'+str(month)
      else:
         month = str(month)
      year = reservationJSON['year']
      day = reservationJSON['day']
      if(day<10):
         day = '0'+str(day)
      else:
         day = str(day)
      startHour = 0
      if(exclusiveReservation[0]<10):
         startHour = '0' + str(exclusiveReservation[0])
      else:
         startHour = str(exclusiveReservation[0])
      msg = "{\"msg\" :\"reservation\",\
      \"start\": \"" + day + "."+ month + "." + year + " " + startHour +":00:00\", \
      \"duration\": " + str(duration) + " \
      }"
      
      sessionSocket.send(msg)
      response = getResponse(sessionSocket)
      response = json.loads(response)
      if(response["value"]):
         logging.info('%s requested reservation on %s.%d.%s since %s:00:00 for %d hours', user, reservationJSON['day'],months.index(reservationJSON['month'])+1 ,reservationJSON['year'], startHour, duration)
      else:
         logging.warning('%s requested reservation on %s.%d.%s since %s:00:00 for %d hours failed', user, reservationJSON['day'],months.index(reservationJSON['month'])+1 ,reservationJSON['year'], startHour, duration)
         returned = False
   return returned
def socketResignation(sessionSocket, resignations, resignationJSON, user):
   duration = 0
   returned = True
   #sprawdzenie danych
   if((not resignationJSON['month'] in months) or (not int(resignationJSON['year']) in range(1900, 2500)) or (not int(resignationJSON['day']) in range(1,31))):
      logging.error('malformed resignation request')
      return False
   for exclusiveResignation in resignations:
      if(not exclusiveResignation[0] in range(0,23)):
         returned = False
         continue
      duration = exclusiveResignation[0]
      month = months.index(resignationJSON['month'])+1
      if(month<10):
         month = '0'+str(month)
      else:
         month = str(month)
      year = resignationJSON['year']
      day = resignationJSON['day']
      if(day<10):
         day = '0'+str(day)
      else:
         day = str(day)
      startHour = 0
      if(exclusiveResignation[0]<10):
         startHour = '0' + str(exclusiveResignation[0])
      else:
         startHour = str(exclusiveResignation[0])
      msg = "{\"msg\" : \"cancel\",\
      \"start\": \"" + day + "."+ month + "." + year + " " + startHour +":00:00\" \
      }"
      
      sessionSocket.send(msg)
      response = getResponse(sessionSocket)
      response = json.loads(response)
      if(response["value"]):
         logging.info('%s requested resignation on %s.%d.%s since %s:00:00 ', user, resignationJSON['day'],months.index(resignationJSON['month'])+1 ,resignationJSON['year'], startHour)
      else:
         logging.warning('%s requested resignation on %s.%d.%s since %s:00:00 failed', user, resignationJSON['day'],months.index(resignationJSON['month'])+1 ,resignationJSON['year'], startHour)
         returned = False
   return returned
#function creating json calendar object

def reservedHours(request):
   sessionSocket = sessions[request.session.session_key]
   sessionSocket.send("{ \"msg\" : \"getCalendar\"}")
   response = getResponse(sessionSocket)
   reservations = json.loads(response)
   returned = reservations["reservations"]
   for i in range(0,len(returned)):
      if(returned[i]["username"] == request.session["username"]):
         returned[i]["username"] = "me"
      else:
         returned[i]["username"] = "someone"
   return JsonResponse({ 'reservations' : returned})

def jsonCalendar(request):
   year = int(request.POST['date[year]'])
   direction = request.POST['dir']
   monthNumber = months.index(request.POST['date[month]'].encode("utf-8"))
   #select direction of calendar change
   if(direction == 'next'):
      monthNumber+=1
   if(direction == 'prev'):
      monthNumber-=1
   #check if month is not out of bounds
   if(monthNumber == -1):
      year -=1
      monthNumber = 11
   if(monthNumber == 12):
      year +=1
      monthNumber = 0
   monthNumber+=1
   logging.info("%s requested calendar for %d.%d", request.session["username"], monthNumber, year)
   return JsonResponse(calendarSite(year, monthNumber))



def calendarSite(year, month):
   
   cal = Calendar()

   weeks = cal.monthdayscalendar(year = year, month = month)
   for week in weeks:
      for i in range(0, len(week)):
         if week[i] == 0:
            week[i] = ""
   return {'year': year, 'month': months[month-1] ,'dayNames' : dayNames, 'weeks' : weeks}



def loginSite(request):
   if(login(request)):
      request.session["username"] = request.POST['user']
      return render(request, 'calendar.html',calendarSite(date.today().year, date.today().month))
def logout(request):
   logging.info("user %s logged out", request.session["username"])
   del request.session["username"]
   if(request.session.session_key in sessions):
      sessions[request.session.session_key].close()
      del sessions[request.session.session_key]
   return

#simple authorization method
def login(request):
   if("username" in request.session):
      return True
   if ('{' in request.POST['user'] or '}' in request.POST['user']):
      logging.error("username with { }")
      return False
   initializeSocket(request)
   if(tryConnect(sessions[request.session.session_key],request.POST['user'], request.POST['password'])):
      logging.info("user %s logged in", request.POST['user'])
      return True
   logging.error("tried to log in as %s failed", request.POST['user'])
   sessions[request.session.session_key].close()
   del sessions[request.session.session_key]
   return False

#function for dividing periods of requested reserved time
def countReservations(timeTable):
   timeTable = timeTable.split()
   for i in range(0,len(timeTable)):
      timeTable[i] = int(timeTable[i].replace(":",""))
      timeTable[i] /= 100
   
   returned = []
   tempArr = []
   tempArr.append(timeTable[0])
   for i in range(1, len(timeTable)):
      #check if next record is this record's hour +1 (if there is any next record)
      if(timeTable[i]-1 == timeTable[i-1]):
         tempArr.append(timeTable[i])
      else:
         returned.append(tempArr)
         tempArr = [timeTable[i]]
   returned.append(tempArr)
   return returned
def initializeSocket(request):
   sessionSocket = socket(AF_INET, SOCK_STREAM)
   sessionSocket.connect((socketAddress, socketPort))
   sessions[request.session.session_key] = sessionSocket
def tryConnect(sessionSocket, username, password):
   sessionSocket.send("{\
    \"msg\": \"handshake\", \
    \"login\": \""+ username +"\"\
    }")
   response = getResponse(sessionSocket)
   response = json.loads(response)
   hashPassword = sha3.SHA3256()
   hashPassword.update(password)
   hashPassword = hashPassword.hexdigest()
   request = response["challenge"] + hashPassword
   hashRequest = sha3.SHA3256()
   hashRequest.update(request)
   hashRequest = hashRequest.hexdigest()
   sessionSocket.send("{\
   \"msg\": \"response\",\
   \"password\" : \"" + hashRequest + "\"\
   }")
   response = getResponse(sessionSocket)
   response = json.loads(response)
   if(response["value"] == True):
      return True
   return False
def getResponse(sessionSocket):
   response = ''
   while(True):
      response = response + sessionSocket.recv(100)
      if(response.count("\"")%2 == 0):
         if(response.count('{')==response.count('}')):
            break
   return response
