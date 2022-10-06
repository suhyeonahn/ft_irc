#include "bircd.h"

void	clean_fd(t_fd *fd)
{
  fd->type = FD_FREE;
  fd->fct_read = NULL;
  fd->fct_write = NULL;
}

void		init_env(t_env *e)
{
    e->maxfd = FD_MAX;
    for (int i ; i < e->maxfd; i++)
        clean_fd(&e->fds[i]);
}

void	get_opt(t_env *e, int ac, char **av)
{
    if (ac != 2)
        exit(1);
    std::sscanf(av[1], "%d", &e->port);
}

void	client_write(t_env *e, int cs)
{
}

void	client_read(t_env *e, int cs)
{
  // Receive a msg from a specific client
  int len = recv(cs, e->fds[cs].buf_read, BUF_SIZE, 0);
  if (len <= 0)
  {
      close(cs);
      clean_fd(&e->fds[cs]);
      printf("client #%d gone away\n", cs);
  }
  else
  {
    for (int i ; i < e->maxfd ; i++)
	  {
      // Send msg to other clients
      if ((e->fds[i].type == FD_CLIENT) && (i != cs))
        send(i, e->fds[cs].buf_read, len, 0);
    }
  }
}

void			srv_accept(t_env *e, int s)
{
  int			cs;
  struct sockaddr_in	csin;
  socklen_t		csin_len;

  csin_len = sizeof(csin);
  cs = accept(s, (struct sockaddr*)&csin, &csin_len);
  printf("New client #%d from %s:%d\n", cs,
	 inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
  clean_fd(&e->fds[cs]);
  e->fds[cs].type = FD_CLIENT;
  e->fds[cs].fct_read = client_read;
  e->fds[cs].fct_write = client_write;
}

void			srv_create(t_env *e, int port)
{
  int			s;
  struct sockaddr_in	sin;
  
  s = socket(PF_INET, SOCK_STREAM, 0);
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);
  bind(s, (struct sockaddr*)&sin, sizeof(sin));
  listen(s, 42);
  e->fds[s].type = FD_SERV;
  e->fds[s].fct_read = srv_accept;
}

void	init_fd(t_env *e)
{
  e->max = 0;
  FD_ZERO(&e->fd_read);
  FD_ZERO(&e->fd_write);
  for (int i = 0 ; i < e->maxfd ; i++)
  {
    if (e->fds[i].type != FD_FREE)
    {
	    FD_SET(i, &e->fd_read);
	    if (strlen(e->fds[i].buf_write) > 0)
	      FD_SET(i, &e->fd_write);
      e->max = MAX(e->max, i);
    }
  }
}

void	check_fd(t_env *e)
{
  for (int i = 0 ; (i < e->maxfd) && (e->r > 0) ; i++)
  {
    if (FD_ISSET(i, &e->fd_read))
	    e->fds[i].fct_read(e, i);
    if (FD_ISSET(i, &e->fd_write))
	    e->fds[i].fct_write(e, i);
    if (FD_ISSET(i, &e->fd_read) || FD_ISSET(i, &e->fd_write))
      e->r--;
  }
}

int	main(int ac, char **av)
{
  t_env	e;

  init_env(&e);
  get_opt(&e, ac, av);
  srv_create(&e, e.port);
  while (1)
  {
    init_fd(&e);
    e.r = select(e.max + 1, &e.fd_read, &e.fd_write, NULL, NULL);
    check_fd(&e);
  }
  return (0);
}
