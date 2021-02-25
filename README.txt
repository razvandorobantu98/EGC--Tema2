Dorobantu Razvan-Florin, 335CB

===============================================================================
Functionalitate joc:

La inceput, jocul porneste la apasarea tastei Space.
Avionul se deplaseaza in functie de miscarile mouse-ului. De fiecare data 
cand avionul se ciocneste de un obstacol, pierde o viata. Cand avionul se 
ciocneste de o bucata de combustibil, nivelul combustibilului creste.
Cand avionul ramane fara vieti sau combustibil, jocul se opreste si avionul 
face animatia de prabusire. La apasarea tastei Space, jocul incepe din nou.

===============================================================================
Fisiere:

Pentru implementarea temei am folosit scheletul Laboratorului 6.
In fisierul Transformari3D.h am scris functiile care returneaza matricile de 
translatie, scalare si rotatie.
In fisierul CameraTema2.h am implementat camera (asemanator ca in Laborator 5).
In VertexShader si FragmentShader am implementat shader-ul folosit pentru toate 
obiectele din scena.
In Tema2.cpp am implementat tema propriu-zisa. 

===============================================================================
Avionul:

Am construit avionul din forme simple (cuburi si sfere). Pentru foma avionului 
am folosit un trunchi de piramida. Astfel avionul este mai ascutit spre coada.
Avionul se misca doar pe axa OY. Factorul de translatie depinde de variabila 
"speed". Daca speed > 0 atunci avionul se deplaseaza in sus, iar daca 
speed < 0 atunci avionul se deplaseaza in jos.
Rotatia avionului depinde de speed, astfel se va inclina mai mult daca speed 
este mai mare.
Speed se modifica in functie de miscarea mouse-ului si de senzitivitate.

===============================================================================
Cilindrul:

Cilindrul e reprezentat de un cilindru (mesh luat de pe net), ce se roteste in 
jurul axei proprii. In VertexShader am o variabila auxiliara care imi spune 
daca vertexul curent apartine cilindrului. In acest caz, modific coordonata Z 
in functie de timeElapsed. astfel, vertexul se va roti in jurul axei OZ, dand 
senzatia de val. 

===============================================================================
Obstacolele:

Un obstacol e reprezentat de o sfera. Intotdeauna vor exista 4 obstacole, ce 
au ca pozitii initiale punctele cardinale relativ la cilindru.
De fiecare data calculez coordonatele X si y pentru fiecare obstacol ce se 
roteste in jurul cilindrului.
La inceput, toate obstacolele vor fi invizibile. Atunci cand un obstacol 
invizibil ajunge in partea de jos a cilindrului, acesta devine vizibil.   
Avionul se poate lovi doar de un obstacol vizibil. In momentul in care avionul 
se loveste de un bostacol, obstacolul devine invizibil.

===============================================================================
Coliziunea:

Am aproximat avionul cu un dreptunghi. De fiecare data cand un obstacol ajunge 
in dreptul avionului (pe axa OX, cu Y > 0) verifiic daca avionul se 
intersecteaza cu un obstacol, caz in care obstacolul devine invizibil si 
avionul pierde o viata.

===============================================================================
Bucatile de combustibil:

Bucatile de combustibil sunt implemenate in acelasi mod ca obstacolele, decat 
ca sunt reprezentate de tetraedre, nu de sfere.
In momentul in care avionul se ciocneste de o bucata de combustibil, 
compustibilul creste si bucata respectiva devine invizibila.

===============================================================================
Norii:

Un nor este format din mai multe cuburi,  care se rotesc in jurul cilindrului. 
Pe langa miscarea in jurul cilindrului, fiecare cub se misca in jurul propriei 
axe.

===============================================================================
Vietile si combustibilul:

Vietile sunt reprezentate de 3 inimi (mesh luat de pe net), care apar pe ecran 
in functie de numarul de vieti ramase.
Combustibilul este reprezentat de un dreptunghi care se scaleaza cu un factor 
ce depinde de nivelul de combustibil ramsa. Astfel, in momentul in care 
avionul ramane fara combustibil, dreptunghiul dispare.
