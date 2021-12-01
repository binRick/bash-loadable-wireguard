

void json_demo(){
  char *json_data =
  "[{\"name\": \"abe lincoln\", \"age\": 21},"
  " {\"name\": \"barb\", \"age\": 56}]";
  struct person { char *name; int age; } people[2];

  json_t json;
  json_load(&json, json_data);

  size_t length;
  json_object_t **objects = json_to_array(json.root, &length);
  int qty = 0;
  for (size_t i = 0; i < length; ++i) {
      people[i].name = strdup(json_get_string(objects[i], "name"));
      people[i].age = (int)json_get_number(objects[i], "age");
      log_info("loaded #%d- %s (age %d)", i, people[i].name, people[i].age);
      qty++;
  }
  log_info("loaded %d People from %d Byte string", qty, strlen(json_data));


  json_unload(&json);
}
