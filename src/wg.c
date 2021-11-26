#include <config.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#if defined (HAVE_UNISTD_H)
#  include <unistd.h>
#endif

#include "builtins.h"
#include "shell.h"
#include "bashgetopt.h"
#include "common.h"

#include "wireguard.h"
#include "wireguard.c"
#include "utils.h"


int dur_demo(){
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	printf("Press ENTER to stop...\n");
	getc(stdin);
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	if(end.tv_nsec < start.tv_nsec){
		end.tv_nsec += 1000000000;
		end.tv_sec--;
	}
	printf("Delta time: [%ld.%09ld]s\n", (long)(end.tv_sec - start.tv_sec), (end.tv_nsec - start.tv_nsec));
	return(0);
}

bool wg_device_exists(char *device_name){
    wg_device *device;
    bool exists = (wg_get_device(&device, device_name) == 0);
    free(device);
    return exists;
}



static int check_keysize(char *key, char *error)
{
	int keysize = (sizeof(wg_key_b64_string) - 1); //Trim the NULL
	int count = 0;

	while(key[count] != '\0')
		count++;

	if (!(count == keysize))
	{
		sprintf(error, "Peer key size wrong got %d instead of %d", count, keysize);
		return count;
	}

	return 0;
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

char * get_wg_interface_name(){
    SHELL_VAR *wg_interface_name = find_variable("WIREGUARD_INTERFACE_NAME");
    if (wg_interface_name != NULL){
      char *ev = get_variable_value(wg_interface_name);
      if (strlen(ev) > 4){
        return ev;
      }
    }
    return DEFAULT_WIREGUARD_INTERFACE_NAME;
}

int wg_set_interface(list) WORD_LIST *list; {

    wg_peer new_peer = {
      .flags = WGPEER_HAS_PUBLIC_KEY | WGPEER_REPLACE_ALLOWEDIPS | WGPEER_HAS_PRESHARED_KEY
    };
    wg_device new_device = {
      .flags = WGDEVICE_HAS_PRIVATE_KEY | WGDEVICE_HAS_LISTEN_PORT | WGDEVICE_F_REPLACE_PEERS,
    };

    SHELL_VAR *wg_interface_name = find_variable("WIREGUARD_INTERFACE_NAME");
    if (wg_interface_name != NULL){
      char *ev = get_variable_value(wg_interface_name);
      if (wg_interface_name != NULL && (strlen(ev) > 4)){
        strcpy(new_device.name, ev);
      }else{
        strcpy(new_device.name, DEFAULT_WIREGUARD_INTERFACE_NAME);
      }
    }else{
      strcpy(new_device.name, DEFAULT_WIREGUARD_INTERFACE_NAME);
    }

    SHELL_VAR *wg_interface_listen_port = find_variable("WIREGUARD_LISTEN_PORT");
    if (wg_interface_listen_port != NULL){
      int iv = atoi(get_variable_value(wg_interface_listen_port));
      if (iv > 1024 && iv < 65536){
        new_device.listen_port =  iv;
      }else{
        new_device.listen_port =  DEFAULT_WIREGUARD_LISTEN_PORT;
      }
    }else{
      new_device.listen_port =  DEFAULT_WIREGUARD_LISTEN_PORT;
    }

    bool key_valid = false;
    wg_key_b64_string key_b64;
    wg_key key;
    int result = 1;
    char *error;
    SHELL_VAR *wg_priv_key = find_variable("WIREGUARD_PRIVATE_KEY");
    if (wg_priv_key != NULL){
      char *wpk = get_variable_value(wg_priv_key);
      if (wpk == NULL){
        key_valid = false;
      }else{
        if ((result = check_keysize((char*)wpk, error)) != 0){
          key_valid = false;
        }else{
          if ((result = wg_key_from_base64(new_device.private_key, wpk)) != 0){
            sprintf(error, "Peer public key error [code: %d]", result);
            key_valid = false;
          }else{
            wg_generate_public_key(new_device.public_key, new_device.private_key);
            key_valid = true;
          }
        }
      }
    }
    if (key_valid == false){
      wg_key temp_private_key;
      wg_generate_private_key(temp_private_key);
      wg_generate_public_key(new_device.public_key, temp_private_key);
      wg_generate_private_key(new_device.private_key);
    }
      wg_key temp_private_key;
      wg_generate_private_key(temp_private_key);
      wg_generate_public_key(new_peer.public_key, temp_private_key);
      wg_generate_private_key(new_peer.preshared_key);
new_device.first_peer = &new_peer;
new_device.last_peer = &new_peer;

		wg_key_b64_string priv, pub;
		wg_key_to_base64(priv, new_device.private_key);
		wg_key_to_base64(pub, new_device.public_key);

    fprintf(stderr, "Managing Wireguard Interface %s listening on port %d\n - private key: %s\n - public key: %s\n - specified key valid: %s\n", 
      new_device.name, 
      new_device.listen_port,
      priv, pub,
      (key_valid == true) ? "true" : "false"
    );


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



int wg_builtin (list) WORD_LIST *list;{
    if (check_no_options(&list) == -1){
      list_devices();
      return (0);
    }
    for (int i = 1; list != NULL; list = list->next, ++i){
        if (strcasecmp(list->word->word, "ls") == 0){
            list_devices();
            return EXECUTION_SUCCESS;
        }
        if (strcasecmp(list->word->word, "dur") == 0){
            dur_demo();
            return EXECUTION_SUCCESS;
        }
        if (strcasecmp(list->word->word, "pwd") == 0){
              SHELL_VAR *pwd = find_variable("PWD");
              if (pwd != NULL){
                fprintf(stdout, "%s\n", get_variable_value(pwd));
                return EXECUTION_SUCCESS;
              }
            return EXECUTION_SUCCESS;
        }
        if (strcasecmp(list->word->word, "up") == 0){
            wg_set_interface();
            return EXECUTION_SUCCESS;
        }
        if (strcasecmp(list->word->word, "down") == 0){
            char *managed_interface = get_wg_interface_name();
            bool device_exists = wg_device_exists(managed_interface);
            fprintf(stderr, "device %s exists? %s\n", managed_interface, device_exists ?  "true" : "false");
            if (!device_exists){
              return EXECUTION_SUCCESS;
            }
            if (wg_del_device(managed_interface) == 0) {
              return EXECUTION_SUCCESS;
            }else{
              fprintf(stderr, "Unable to delete device %s\n", managed_interface);
              return(1);
            }
        }
    }
    printf("Unhandled Mode\n");
    fflush (stdout);
    return (EXECUTION_SUCCESS);
}

int wg_builtin_load (s) char *s; {
  printf ("wg builtin loaded\n");
  fflush (stdout);
  return (1);
}

void wg_builtin_unload (s) char *s; {
  printf ("wg builtin unloaded\n");
  fflush (stdout);
}

char *wg_doc[] = {
	"Wireguard Bash Builtin.",
	"",
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
