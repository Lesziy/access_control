#-*- coding: utf-8 -*-
from django.shortcuts import render
from django.template import RequestContext
from django.shortcuts import render_to_response
from django.http import HttpResponse, JsonResponse
from calendar import Calendar
from datetime import date
months = ['Styczeń', 'Luty', 'Marzec', 'Kwiecień', 'Maj', 'Czerwiec', 'Lipiec', 'Sierpień', 'Wrzesień', 'Pazdziernik', 'Listopad', 'Grudzień']
dayNames = ['Pon', 'Wt', 'Śr', 'Czw','Pt','Sob','Nie']
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
         return jsonCalendar(int(request.POST['date[year]']),request.POST['date[month]'], request.POST['dir'])
   #hold session
   
   if "username" in request.session:
      print (request.session["username"] + " is logged in")
      return render(request, 'calendar.html',calendarSite(date.today().year, date.today().month))
   #basic login template 
   return render(request, 'reservationSite.html')

#function creating json calendar object

def jsonCalendar(year, month, direction):
   monthNumber = months.index(month.encode("utf-8"))
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
   del request.session["username"]
   return

#simple authorization method
def login(request):
   if(request.POST['user'] == 'admin' and request.POST['password'] == 'admin'):
      return True
   print "tried to log in as " + request.POST['user'] + " failed"
   return False