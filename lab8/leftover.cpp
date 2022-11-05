    // int leftover = size - pow(2, total_stages-1);
    // if (rank < leftover) {
    //     // send
    //     // printf("send %d --> %d\n", rank, int(pow(2, total_stages-1) + rank));
    //     MPI_Send(buffer, count, datatype, int(pow(2, total_stages-1) + rank), 777, comm);
    // }
    // else if (rank >= pow(2, total_stages-1)) {
    //     // recv
    //     MPI_Status stat;
    //     // printf("recv %d <-- %d\n", rank, int(rank - pow(2, total_stages-1)));
    //     MPI_Recv(buffer, count, datatype, int(rank - pow(2, total_stages-1)), MPI_ANY_TAG, comm, &stat);
    // }