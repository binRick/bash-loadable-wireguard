#include <config.h>
#if defined (HAVE_UNISTD_H)
#  include <unistd.h>
#endif
#include <stdio.h>
#include "builtins.h"
#include "shell.h"
#include "bashgetopt.h"
#include "common.h"

#include "wireguard.h"
#include <string.h>
#include <stdlib.h>
#include "wireguard.c"



bool wg_device_exists(char *device_name){
    wg_device *device;
    bool exists = (wg_get_device(&device, device_name) == 0);
    free(device);
    return exists;
}

void list_devices(void) {
  char *device_names, *device_name;
  size_t len;

  device_names = wg_list_device_names();
  if (!device_names) {
    perror("Unable to get device names");
    exit(1);
  }

  wg_for_each_device_name(device_names, device_name, len) {
    wg_device *device;
    wg_peer *peer;
    wg_key_b64_string key;

    if (wg_get_device(&device, device_name) < 0) {
      perror("Unable to get device");
      continue;
    }
    if (device->flags & WGDEVICE_HAS_PUBLIC_KEY) {
      wg_key_to_base64(key, device->public_key);
      printf("%s has public key %s\n", device_name, key);
    } else
      printf("%s has no public key\n", device_name);
    wg_for_each_peer(device, peer) {
      wg_key_to_base64(key, peer->public_key);
      printf(" - peer %s\n", key);
    }
    wg_free_device(device);
  }
  free(device_names);
}


int wg_set_interface(list) WORD_LIST *list; {

  wg_peer new_peer = {
    .flags = WGPEER_HAS_PUBLIC_KEY | WGPEER_REPLACE_ALLOWEDIPS
  };
  wg_device new_device = {
   // .name = &new_wg_interface_name,
    .listen_port = DEFAULT_WIREGUARD_LISTEN_PORT,
    .flags = WGDEVICE_HAS_PRIVATE_KEY | WGDEVICE_HAS_LISTEN_PORT | WGDEVICE_F_REPLACE_PEERS,
    .first_peer = &new_peer,
    .last_peer = &new_peer
  };

    //char *new_wg_interface_name = "";
    SHELL_VAR *wg_interface_name = find_variable("WIREGUARD_INTERFACE_NAME");
    if (wg_interface_name != NULL){
      char *ev = get_variable_value(wg_interface_name);
      strcpy(new_device.name, ev);
    }else{
      strcpy(new_device.name, DEFAULT_WIREGUARD_INTERFACE_NAME);
    }
    fprintf(stderr, "Managing Wireguard Interface %s\n", new_device.name);

//return 0;

  wg_key temp_private_key;
  wg_generate_private_key(temp_private_key);
  wg_generate_public_key(new_peer.public_key, temp_private_key);
  wg_generate_private_key(new_device.private_key);

  bool device_exists = wg_device_exists(new_device.name);
  fprintf(stderr, "device %s exists? %s\n", new_device.name, device_exists ?  "true" : "false");

  if (device_exists && RECREATE_WIREGUARD_INTERFACE){
    if (wg_del_device(new_device.name) < 0) {
      perror("Unable to delete device");
      exit(1);
    }
  }else if(!device_exists){
    if (wg_add_device(new_device.name) < 0) {
        perror("Unable to add device");
        exit(1);
    }
  }

  if (wg_set_device(&new_device) < 0) {
    perror("Unable to set device");
    exit(1);
  }

  return 0;
}



int
wg_builtin (list)
     WORD_LIST *list;
{
  printf("wg world\n");
  fflush (stdout);
  return (EXECUTION_SUCCESS);
}

int
wg_builtin_load (s)     char *s; {


  printf ("wg builtin loaded\n");
  fflush (stdout);
    wg_set_interface();
    list_devices();
  return (1);
}

void
wg_builtin_unload (s)
     char *s;
{
  printf ("wg builtin unloaded\n");
  fflush (stdout);
}

char *wg_doc[] = {
	"Sample builtin.",
	"",
	"this is the long doc for the sample wg builtin",
	(char *)NULL
};

struct builtin wg_struct = {
	"wg",
	wg_builtin,
	BUILTIN_ENABLED,
	wg_doc,
	"wg",
	0	
};
