from django.conf.urls import patterns, url
from djApplication import views
urlpatterns = [
	url(r'^$', views.index),
			  ]