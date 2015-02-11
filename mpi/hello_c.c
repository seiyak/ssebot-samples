/*
 * Copyright (c) 2004-2006 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2006      Cisco Systems, Inc.  All rights reserved.
 *
 * Sample MPI "hello world" application in C
 */

#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
    int rank, size, len,plen;
    char version[MPI_MAX_LIBRARY_VERSION_STRING];
    char pname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_library_version(version, &len);
    MPI_Get_processor_name(pname,&plen);
    printf("Hello, world, I am %d of %d, (%s, %d) with CPU: %s\n", 
           rank, size, version, len,pname);
    MPI_Finalize();

    return 0;
}
