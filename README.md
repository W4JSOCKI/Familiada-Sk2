# Familiada-Sk2


Projekt na sieci komputerowe 2 
autorstwa
Patryk Wojtalak I7 148236
Stefan Zbąszyniak I2.2 148078

# Opis

Gracz łączy się do serwera i wysyła swój nick (jeśli nick jest już zajęty,
serwer prosi o podanie innego nicku) gracze zostają przydzieleni do pierwszej
wolnej drużyny w danym lobby. Jeżeli pokój jest pełny to zostaje utworzony nowy.

Jeżeli nie ma wystarczającej ilości graczy w którym widzi liczbę graczy w lobby
i ma możliwość opuszczenia lobby.

Po uzbieraniu odpowiedniej ilości graczy gra się rozpoczyna.

Wyświetla się pierwsze pytatnie i pierwszy gracz z danej drużyny ma możliwość
naciśnięcia przycisku. Szybszy z nich ma możliwość odpowiedzi na pytanie. 
Jeżeli odpowiedź jest prawidłowa i najwyżej punktowana to drużyna tego gracza
przejmuje aktualną kategorię. Jeżeli nie jest najwyżej punktowana (może być błędna) 
to przeciwnikteż może odpowiedzieć na pytanie. Kategorię przejmuje drużyna osoby z 
wyżej punktowaną odpowiedzią.
Jeżeli obie odpowiedzi są niepoprawne to pytani są na zmiane kolejni członkowie
drużyn da których należą dani gracze zaczynając od drużyny tego który pierwszy 
nacisnął przycisk. Po kolejnych czterech niepoprawnych odpowiedziach kategoria jest zmieniana.

Drużyna która przejeła kategorię po kolei odpowiada na pytanie. Poprawne odpowiedzi 
są wyświetlane na tablicy wraz z liczbą punktów. Jeżeli gracze podadzą 3 niepoprawne
odpowiedzi to przeciwna drużyna przejmuje kategorię, ale przestają zgadywać po jednej niepoprawnej odpowiedzi.

Punktacja w pierwszych trzech rundach pozostaje standardowa, w czwartej rundzie liczbę punktów mnoży się przez 2, a od piątej mnoży się przez 3.

Wygrywa drużyna mająca więcej punktów.

Po zakończonej rozgrywce jeeżeli nikt nie opuści lobby gra rozpoczyna się ponownie po określonym czasie.

Jeżeli gracz nie udzieli odpowiedzi we wskazanym czasie jest to liczone jako błąd. 
W przypadku utraty połączenia gracz będzie udzialał samych błędnych odpowiedzi,
a po zakończeniu rozgrywki zostanie wyrzucony z lobby.
