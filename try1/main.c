#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <SDL.h>
#include "render.h"
#include "db.h"
#include <mongoose.h>

#define INPUT_EQ(s) (strcmp((input), (s)) == 0)

static int repl(void *ptr) {
  int run = 1;
  while(run) {
    printf("> ");
    char input[1024];
    fgets(input, 1024, stdin);

    if(INPUT_EQ("q\n")) {
      run = 0;
      quit = 1;
    }
    else if(INPUT_EQ("r\n")) {
      performReload = 1;
    }
    else {
      printf("Can't understand command: %s\n", input);
    }    
  }
  return 0;
}

static const char *s_http_port = "1234";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) p;
    mg_send_head(c, 200, hm->message.len, "Content-Type: text/plain");
    
    if(mg_vcmp(&hm->uri, "/yo") == 0) {
      printf("It's a yo.\n");
      mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n%.*s",
                (unsigned long) hm->body.len, (int) hm->body.len, hm->body.p);
    } else {      
      mg_printf(c, "%.*s", (int)hm->message.len, hm->message.p);
    }
  }
}

int server(void *ptr) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, NULL);
  printf("Starting web server on port %s\n", s_http_port);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "no";

  //system("open http://localhost:1234");

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}

int main() {
  SDL_Window *win = render_setup();
  SDL_Thread *replThread = SDL_CreateThread(repl, "repl", NULL);
  SDL_Thread *serverThread = SDL_CreateThread(server, "server", NULL);
  return render_go(win);
}
