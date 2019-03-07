#include <atomic>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <atomic>
#define SEM_NAME "semaphore_example_1"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 0

int main(int argc, char *argv[])
{
  int fd;
  fd = open("user.dat", O_RDWR);
  if (fd == -1)
  {
    fd = open("user.dat", O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
      exit(-1);
    }
  }
  struct stat st;
  int r = fstat(fd, &st);
  if (r == -1)
  {
    close(fd);
    exit(-1);
  }
  int len = 4096; //st.st_size;
  ftruncate(fd, 4096);
  char *p = (char*)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (p == NULL || p == (void *)-1)
  {
    close(fd);
    exit(-1);
  }
  //
  std::atomic_int *ato = new (p) std::atomic_int;
  int * ito = new (p+2048) int;
  ato->store(0);
  *ito = 0;

  std::cout << " atomic int:" << ato->load() << std::endl;
  std::cout << " int :" << *ito << std::endl;

  int ABtoC[2];
  pipe(ABtoC);

  sem_t *semaphore = sem_open(SEM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, INITIAL_VALUE);
  // sem_t semaphore;
  // int ret = sem_init(&semaphore, 0, 0);
  if (!semaphore)
  {
    perror("sem_open error");
    exit(EXIT_FAILURE);
  }

  const int count = 10;
  pid_t pids[count];
  size_t i;

  for (i = 0; i < sizeof(pids) / sizeof(pids[0]); i++)
  {
    if ((pids[i] = fork()) < 0)
    {
      perror("sem_open error");
      exit(EXIT_FAILURE);
    }

    if (pids[i] == 0)
    {
      close(ABtoC[0]);
      dup2(ABtoC[1], STDOUT_FILENO);
      close(ABtoC[1]);

      std::cout << "subprocess waiting." << std::endl;
      sem_wait(semaphore);
      sleep(1);

      int t = 100000000;
      while (t--)
      {
        (*ato).fetch_add(1);
        (*ito)++;
      }
      std::cout << getpid() << " atomic int:" << ato->load() << std::endl;
      std::cout << getpid() << " int:" << *ito << std::endl;

      exit(0);
    }
    else
    {
      std::cout << getpid() << "sub process:" << pids[i] << std::endl;
    }
  }
  sleep(1);
  std::cout << " atomic int:" << ato->load() << std::endl;
  std::cout << " int:" << *ito << std::endl;

  for (int ind = 0; ind < count; ind++)
  {
    sem_post(semaphore);
  }

  char buffer[100];
  ssize_t nbytes;
  close(ABtoC[1]);
  dup2(ABtoC[0], STDIN_FILENO);
  close(ABtoC[0]);

  while ((nbytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
    write(STDOUT_FILENO, buffer, nbytes);

  munmap(p, len);
  close(fd);

  for (i = 0; i < sizeof(pids) / sizeof(pids[0]); i++)
    if (waitpid(pids[i], NULL, 0) < 0)
    {
      exit(EXIT_FAILURE);
    }

  if (sem_close(semaphore) < 0)
  {
    perror("sem_open error");
    exit(EXIT_FAILURE);
  }
  if (sem_unlink(SEM_NAME) < 0) {
    perror("sem_open error");

  }

  return 0;
}
