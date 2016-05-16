
from django.shortcuts import render
from django.template import RequestContext
from django.shortcuts import render_to_response
from django.http import HttpResponse

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
		return render(request, 'index.html')
	#basic login template 
	return render(request, 'reservationSite.html')
	


def loginSite(request):
	if(login(request)):
		request.session["username"] = request.POST['user']
		return render(request, 'index.html')
	
def logout(request):
	del request.session["username"]
	return

#simple authorization method
def login(request):
	if(request.POST['user'] == 'admin' and request.POST['password'] == 'admin'):
		return True
	return False