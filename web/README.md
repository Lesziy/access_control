## Opis
Projekt serwera kontroli dostępu do protokołu **SSH**

## ver0.0001a
Do uruchomienia serwera służy polecenie **python manage.py runserver** {opcjonalnie "... runserver:numer_portu"}.

Przed uruchomieniem, jeśli nie istnieje plik db.sqlite3, należy wpisać **python manage.py migrate**

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

**testowane na Firefox 45.0 na linuxie mint 17.1 cinnamon**
##Szczegóły instalacji (na podstawie linux mint 17.1 cinnamon)
1. Potrzebujemy **pythona 2.7**, **pip** i **apache2**<br />
`sudo apt-get install python`<br />
`sudo apt-get install python-pip`<br />
`sudo apt-get install apache2`<br />
 - do apache ściągamy **mod-wsgi**<br />
 `sudo apt-get install libapache2-mod-wsgi`<br />
 - musimy ściągnąć moduły **django**, **sha3** i **django-ipware** używając **pip**<br />
 `sudo pip install django`<br />
 `sudo pip install django-ipware`<br />
   - do sha3 potrzebujemy nagłówków deweloperskich pythona:<br />
    `sudo apt-get install python-dev`<br />
    `sudo pip install sha3`<br />
2. konfiguracja serwera **apache2**:
  * do uruchomienia serwera potrzebujemy klucza i certyfikatu
  generujemy je komendą `sudo openssl req -x509 -newkey rsa:2048 -keyout /etc/apache2/ssl/server.key -out /etc/apache2/ssl/server.crt`
lub wklejamy już posiadane do folderu `/etc/apache2/ssl/`
  * musimy zmodyfikować plik
   `web/apache2sites/testSite.conf`
    zamieniając ścieżki do plików projektu na własne tzn.
      - w linii 12 <br />
        `Alias /static /home/lesziy/djTest/djTestProject/web/static`<br />
      na:<br />
        `Alias /static [nasza ścieżka]/web/static`
      - w linii 13 <br />
        `<Directory /home/lesziy/djTest/djTestProject/web/static>`<br />
      na: <br />
        `<Directory [nasza ścieżka]/web/static>` 
      - w linii 24 <br />
        `WSGIDaemonProcess djTest python-path=/home/lesziy/djTest/djTestProject/web` <br />
        na: <br />
        `WSGIDaemonProcess djTest python-path=[nasza ścieżka]/web`
      - w linii 26 <br />
         `WSGIScriptAlias / /home/lesziy/djTest/djTestProject/web/djTestProject/wsgi.py`<br />
      na:<br />
         `WSGIScriptAlias / [nasza ścieżka]/web/djTestProject/wsgi.py`<br />
    po czym wystarczy go zapisać i przenieść razem z `defaulttestSite.conf` do folderu `/etc/apache2/sites-available`
  * musimy uruchomić moduły ssl i wsgi. Robimy to, wpisując
    `sudo a2enmod ssl`<br />
    `sudo a2enmod wsgi`<br />
  * trzeba dodatkowo dla całego katalogu web zmienić uprawnienia dostępu tak, żeby serwer mógł się do nich dostać: <br />
    `sudo chown -R :www-data ./web` - w folderze projektu <br />
    `sudo chmod -R 777 ./web` - w folderze projektu <br />
  * musimy aktywować nasze strony: <br />
    `sudo a2ensite defaulttestSite.conf`
    `sudo a2ensite testSite.conf`
  * teraz możemy wpisać `sudo service apache2 restart`, wpisać hasło do swojego klucza prywatnego i voila! <br />
   serwer dostępny jest pod adresem **127.0.0.1  (localhost)**
3. konfiguracja aplikacji django <br />
  konfiguracja django jest na szczęście o wiele mniej pracochłonna
   * należy jedynie w pliku `web/djApplication/views.py` w linii **19 i 20** wprowadzić adres sieciowy serwera app_server
   * na środowisku produkcyjnym serwer powinien działać z opcją Debug = False w pliku `web/djTestProject/settings.py` (linia 26)
4. **dodatkowe informacje**:
  - do udostępnienia serwera w sieci lokalnej musimy otworzyć porty 80 i 443 w iptables, możemy to zrobić wpisując <br />
  `-A INPUT -p tcp --dport 80 -j ACCEPT` <br />
  `-A INPUT -p tcp --dport 443 -j ACCEPT` <br />
  - żeby udostępnić serwer w sieci globalnej, należy w ustawieniach routera przekierować protokół TCP z portów 80 i 443
   na lokalny adres ip serwera
  - serwer apache2 **musi** być jedyną aplikacją na danym komputerze, która używa portów 80 i 443
  - żeby dodać do serwera certyfikat CA należy pod linią `SSLEngine on` dodać linię `SSLCertificateChainFile /etc/apache2/ssl/CA.crt` i plik z certyfikatem umieścić w folderze `/etc/apache2/ssl/` pod nazwą **CA.crt**
  - serwer, do celów testowych można uruchomić również z lokalizacji **/web** wpisując `python manage.py runserver`
  - serwer do działania wymaga uruchomionego serwera app_server<br />
    
