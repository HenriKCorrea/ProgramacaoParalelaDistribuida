#include "MasterProcess"

void MasterProcess::run()
{
        std::cout << "O mestre chegou!" << std::endl << "Hoje la na casa do seu Ze vai rolar uma ousadia!" << std::endl;
        for (source = 1; source < proc_n; source++)
        {
            MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
            printf("%s\n", message);
        }
}