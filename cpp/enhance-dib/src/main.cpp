// stitching.cpp : Defines the entry point for the console application.
//

#include "CmdUtil.h"

int _tmain(int argc, TCHAR **argv)
{
	vks_cmd::CmdUtil util;
	util.ProcessArg(argc, argv);
	return 0;
}

