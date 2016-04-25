## Opis
Projekt serwera kontroli dostępu do protokołu **SSH**

## ver0.0001a
Do uruchomienia serwera służy polecenie **python manage.py runserver** {opcjonalnie "... runserver:numer_portu"}.

**W dalszej części przyjmuję, że aplikacja została uruchomiona na porcie 8000**

Po uruchomieniu serwera za pomocą przeglądarki otwieramy stronę **http://localhost:8000**. Strona na chwilę obecną pozwala jedynie zalogować i wylogować się.
**Dane logowania:**
**login:** admin
**password:** admin
Możemy również zalogować się do panelu administracyjnego na stronie **http://localhost:8000/admin** przy użyciu tych samych danych logowania.

##Instalacja

Do uruchomienia aplikacji wymagane są:
* python 2.7+
* django 1.9.5+
