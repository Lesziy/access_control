#-*- coding: utf-8 -*-
from django.shortcuts import render
from django.template import RequestContext
from django.shortcuts import render_to_response
from django.http import HttpResponse, JsonResponse
from calendar import Calendar
from datetime import date
import json
import logging
months = ['Styczeń', 'Luty', 'Marzec', 'Kwiecień', 'Maj', 'Czerwiec', 'Lipiec', 'Sierpień', 'Wrzesień', 'Październik', 'Listopad', 'Grudzień']
dayNames = ['Pon', 'Wt', 'Śr', 'Czw','Pt','Sob','Nie']
logging.basicConfig(filename = '../web/logs/log.log',level = logging.DEBUG, format = '%(asctime)s %(levelname)s: %(message)s',datefmt = '%m/%d/%Y %H:%M:%S')
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
         
   #hold session
   
   if "username" in request.session:
      return render(request, 'calendar.html',calendarSite(date.today().year, date.today().month))
   #basic login template 
   return render(request, 'reservationSite.html')

# for now only stub, but socket function requesting reservation0
def processReservation(request):
   reservation = json.loads(request.POST['reserve'])
   ##convert request to utf-8
   reservation['month'] = reservation['month'].encode("utf-8")
   time = countReservations(reservation['time'])
   for exclusiveReservation in time:
      logging.info('%s requested reservation on %s.%d.%s from %s to %s', request.session["username"], reservation['day'],months.index(reservation['month'])+1 ,reservation['year'],exclusiveReservation[0],exclusiveReservation[-1])
   return "reservation accepted"

#function creating json calendar object

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


def loginSite(request):
   if(login(request)):
      request.session["username"] = request.POST['user']
      return render(request, 'calendar.html',calendarSite(date.today().year, date.today().month))
def calendarSite(year, month):
   
   cal = Calendar()

   weeks = cal.monthdayscalendar(year = year, month = month)
   for week in weeks:
      for i in range(0, len(week)):
         if week[i] == 0:
            week[i] = ""
   return {'year': year, 'month': months[month-1] ,'dayNames' : dayNames, 'weeks' : weeks}

def logout(request):
   logging.info("user %s logged out", request.session["username"])
   del request.session["username"]
   return

#simple authorization method
def login(request):
   if(request.POST['user'] == 'admin' and request.POST['password'] == 'admin'):
      logging.info("user %s logged in", request.POST['user'])
      return True
   logging.error("tried to log in as %s failed", request.POST['user'])
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



