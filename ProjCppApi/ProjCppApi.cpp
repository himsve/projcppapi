
#pragma once

#include "pch.h"
#include <iostream>
#include <proj.h>
#include <sqlite3.h>
 
int main()
{
	PJ *P = nullptr;
	PJ *Q = nullptr;
	PJ *R = nullptr;

	PJ_COORD c;
	PJ_CONTEXT *m_ctxt = nullptr;
 
	c.lpzt.lam = 0.175; // radians
	c.lpzt.phi = 1.05; // radians
	c.lpzt.z = 100.0;
	c.lpzt.t = 2000.0;

	m_ctxt = proj_context_create();

	const char *const paths[] = { "C:/Users/himsve/Miniconda3/share/proj" };
	proj_context_set_search_paths(m_ctxt, 1, paths); 
 
	std::cout << "EUREF89: lat lon orto. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;

	P = proj_create(m_ctxt, "+proj=vgridshift +grids=egm96_15.gtx");
	c = proj_trans(P, PJ_INV, c);
	std::cout << "EUREF89: lat lon ell. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;

	Q = proj_create(m_ctxt, "+proj=hgridshift +grids=EUREF89_NGO48_20081014.ct2 +ellps=GRS80");
	c = proj_trans(Q, PJ_FWD, c);
	std::cout << "NGO48: lat lon ell. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;

	c = proj_trans(Q, PJ_INV, c);
	std::cout << "EUREF89: lat lon ell. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;

	R = proj_create(m_ctxt, "+proj=utm +zone=32 +ellps=GRS80");
	c = proj_trans(R, PJ_FWD, c);
	std::cout << "EUREF89: east north ell. height: " << c.xyz.x << " " << c.xyz.y << " " << c.xyz.z << std::endl;

	if (P == 0)
		return 1; 

	if (Q == 0)
		return 1;

	if (R == 0)
		return 1;
	 
	proj_destroy(P);
	proj_destroy(Q);
	proj_destroy(R);

	std::cout << "Press Enter to continue...";
	std::cout.flush();
	getwchar();
}
