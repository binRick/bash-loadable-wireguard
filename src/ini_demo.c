
int ini_demo(int argc, char **argv){
  ini_t      *config1    = ini_load("/etc/wireguard/guard0.conf");
  const char *PrivateKey = ini_get(config1, "Interface", "PrivateKey");

  if (PrivateKey)
  {
    printf("[INI]    PrivateKey: %s\n", PrivateKey);
  }
  ini_t      *config = ini_load("./test.ini");
  const char *name   = ini_get(config, "owner", "name");

  if (name)
  {
    printf("[INI]    name: %s\n", name);
  }
  return(0);
}
