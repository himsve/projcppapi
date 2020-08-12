
#pragma once

#include <iostream>
#include <proj.h>
#include <sqlite3.h>
#include <windows.h>

int main()
{ 
	PJ *P = nullptr;
	PJ *Q = nullptr;
	PJ *R = nullptr;

	PJ_COORD c;
	PJ_CONTEXT *m_ctxt = nullptr;	
	
	// Typedeklarasjon av metodepeikarar
	typedef PJ_COORD(PROJCOORD)(double, double, double, double);
	typedef PJ_COORD(PROJTRANS)(PJ*, PJ_DIRECTION, PJ_COORD);
	typedef PJ*(PROJCREATE)(PJ_CONTEXT*, const char *);
	typedef PJ*(PROJDESTROY)(PJ*);
	typedef PJ_CONTEXT*(PROJCONTEXTCREATE)(void);
	typedef void*(PROJCONTEXTSETSEARCHPATHS)(PJ_CONTEXT*, int, const char* const*);
 	
	PROJCOORD* pProjCoord;
	PROJTRANS* pProjTrans;
	PROJCREATE* pProjCreate;
	PROJDESTROY* pProjDestroy;
	PROJCONTEXTCREATE* pProjContextCreate;
	PROJCONTEXTSETSEARCHPATHS* pProjContextSetSearchPaths;
	
	// Import av proj dll
 	HINSTANCE hGetProcIDDLL = LoadLibraryA("proj_7_1.dll");
	
	if (!hGetProcIDDLL)
		return 1;

	pProjCoord = (PROJCOORD*)::GetProcAddress(hGetProcIDDLL, "proj_coord");
	pProjTrans = (PROJTRANS*)::GetProcAddress(hGetProcIDDLL, "proj_trans");
	pProjCreate = (PROJCREATE*)::GetProcAddress(hGetProcIDDLL, "proj_create");
	pProjDestroy = (PROJDESTROY*)::GetProcAddress(hGetProcIDDLL, "proj_destroy");
	pProjContextCreate = (PROJCONTEXTCREATE*)::GetProcAddress(hGetProcIDDLL, "proj_context_create");
	pProjContextSetSearchPaths = (PROJCONTEXTSETSEARCHPATHS*)::GetProcAddress(hGetProcIDDLL, "proj_context_set_search_paths");
	
	if (!pProjCoord)
		return 1;

	if (!pProjTrans)
		return 1;

	if (!pProjCreate)
		return 1;

	if (!pProjDestroy)
		return 1;

	if (!pProjContextCreate)
		return 1;

	if (!pProjContextSetSearchPaths)
		return 1;

	// Input som lengde (rad), breidde (rad), høgde (m), epoke (år)
	c = (*pProjCoord)(0.175, 1.05, 100.0, 2000.0);

	m_ctxt = (*pProjContextCreate)();

	// Set path til ressursfiler, t.d. ctx- og ct2-filer.
	const char *const paths[] = { "C:/Users/himsve/Miniconda3/share/proj" };
	(*pProjContextSetSearchPaths)(m_ctxt, 1, paths);

	std::cout << "EUREF89: lat lon orto. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;

	// Transformasjon av ortometrisk høgde til ellipsoidisk høgde. 
	P = (*pProjCreate)(m_ctxt, "+proj=vgridshift +grids=egm96_15.gtx");
	c =	(*pProjTrans)(P, PJ_INV, c);
	std::cout << "EUREF89: lat lon ell. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;
	
	// Transformasjon av geografiske koordinatar frå EUREF89 til NGO48.
	Q = (*pProjCreate)(m_ctxt, "+proj=hgridshift +grids=EUREF89_NGO48_20081014.ct2 +ellps=GRS80");
	c = (*pProjTrans)(Q, PJ_FWD, c);
	std::cout << "NGO48: lat lon ell. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;
	
	// Transformasjon av geografiske koordinatar frå NGO48 til EUREF89.
	c = (*pProjTrans)(Q, PJ_INV, c);
	std::cout << "EUREF89: lat lon ell. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;

	// Transformasjon av geografiske koordinatar frå EUREF89 til UTM32.
	R = (*pProjCreate)(m_ctxt, "+proj=utm +zone=32 +ellps=GRS80");
	c = (*pProjTrans)(R, PJ_FWD, c);
	std::cout << "EUREF89: east north ell. height: " << c.xyz.x << " " << c.xyz.y << " " << c.xyz.z << std::endl;
	
	if (P == 0)
		return 1;

	if (Q == 0)
		return 1;

	if (R == 0)
		return 1;
		
	(*pProjDestroy)(P);
	(*pProjDestroy)(Q);
	(*pProjDestroy)(R);

	std::cout << "Press Enter to continue...";
	std::cout.flush();
	getwchar();
}
