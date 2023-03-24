# projcppapi

Repoet *projcppapi* er ein wrapper rundt Proj i C++ managed code, C++ unmanaged code og c# .net og .netcore. projcppapi er utvikla i VS2022 Pro. Solution kallar C-API'et i Proj.

## Installasjon av Proj

Fyrst, installer Proj, gjerne i Anaconda. 

``conda install -c conda-forge proj``

## Miljøvariablar 

VS-solution forventar at miljøvariabelen *PROJ_DATA* er sett.

Eksempel:

``set PROJ_DATA=<datafolder>\Library\share\proj``

## Synking av ressursfiler

Norske ressursfiler må synkast frå proj.cdn.org. Mange av transformasjonane er avhengig av at ressursfilene ligg nedlasta lokalt.

Synking av norske filer:

``projsync --bbox 4,57,33,82``
 

Denne Visual Studio solution inneheld testar med kall mot Proj sitt API. 

### ProjCppApi:

I dette prosjektet er Proj-API'et linka inn som lib. Namnet på det statisk bibliotektet er proj.lib, eller proj_d.lib for debug. Prosjektet blir kompilert både som unmanaged c++.

### ProjCppApiCore

Projektet er ProjCppApi som unmanaged i rammeverket .netcore 3.1.


### ProjCoreApi

*ProjCoreApi* er c#-kode som brukar *ProjCppApiCore*.


### ProjCppApiNet

*ProjCppApiNet* er c#-kode som brukar *ProjCppApiCore* i rammeverket .net v4.7.2.


### ProjCppApiTest

Prosjektet *ProjCppApiTest* er stress- og enheitstestar av ProjCppApi


### AspCoreWebApi

Under utvikling

### AspCoreWebApp

Under utvikling
