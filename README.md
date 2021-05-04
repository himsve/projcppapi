# projcppapi

Denne Visual Studio solution inneheld testar med kall mot Proj sitt API.



### ProjCppApiLinkedDll:

I dette prosjektet er Proj-API'et linka inn som dll. Namnet på dll'en som er brukt er proj_7_1.dll.

Includefolderen (..\Library\include) til Proj må endrast i "Additional Include Directories" i Visual Studio, t.d. "C:\Users\himsve\Miniconda3\Library\include".
Pathen til ressursfolderen (..\share\proj) må også endrast. Pathen er definert som "resourcePath" i ProjCppApi.cpp.



### ProjCppApiLinkedLib:

I dette prosjektet er Proj-API'et linka inn som lib. Namnet på det statisk bibliotektet er proj.lib, eller proj_d.lib for debug. 

Includefolderen (..\Library\include) til Proj må endrast i "Additional Include Directories" i Visual Studio, t.d. "C:\Users\himsve\Miniconda3\Library\include".
Libfoldern (..\Library\lib) til Proj må endrast i "Library Directories" i VS, t.d. "C:\Users\himsve\Miniconda3\Library\lib";
Pathen til ressursfolderen (..\share\proj) må også endrast. Pathen er definert som "resourcePath" i ProjCppApi.cpp.
Lib-fila må også refererast fysisk i Visual Studio. Fila skal referast i "Addition Dependencies" i VS, t.d. "C:\Users\himsve\Miniconda3\Library\lib\proj.lib".

Dette VS-prosjektet vil ikkje fungere utan debugversjonen av det statisk biblioteket. Namnet på fila er "proj_d.lib". 


### Resources:

Dette repositoriet inneheld flg. ressursfiler:

- EUREF89_NGO48_20081014.ct2 (2D-transformasjon EUREF89->NGO48)


### Pathar

I Windows må ein leggje inn flg. pathar i "Environment variblar"

- ..\Library\bin (t.d "C:\Users\himsve\Miniconda3\Library\bin")
- ..\Library\lib (t.d "C:\Users\himsve\Miniconda3\Library\lib")
- ..\share\proj (t.d. "C:\Users\himsve\Miniconda3\share\proj")
 









