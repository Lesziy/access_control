#-*- coding: utf-8 -*-
from django.shortcuts import render
from django.template import RequestContext
from django.shortcuts import render_to_response
from django.http import HttpResponse
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
	#hold session
	if "username" in request.session:
		print (request.session["username"] + " is logged in")
		return calendarSite(request, date.today().year, date.today().month-1)
	#basic login template 
	return render(request, 'reservationSite.html')
	


def loginSite(request):
	if(login(request)):
		request.session["username"] = request.POST['user']
		return calendarSite(request, date.today().year, date.today().month-1)
def calendarSite(request, year, month):
   
   cal = Calendar()

   weeks = cal.monthdayscalendar(year = year, month = month)
   
   return render(request,'calendar.html',{'year': year, 'month': months[month] ,'dayNames' : dayNames, 'weeks' : weeks})

def logout(request):
	del request.session["username"]
	return

#simple authorization method
def login(request):
	if(request.POST['user'] == 'admin' and request.POST['password'] == 'admin'):
		return True
	return False