

void json_demo(){
  char *json_data_obj = "{\"name\": \"abe lincoln\", \"age\": 21}";
  char *json_data_obj1 = "{\"dat\": {\"name\": \"abe lincoln\", \"age\": 21}}";

  char *json_data =
  "[{\"name\": \"abe lincoln\", \"age\": 21},"
  " {\"name\": \"barb\", \"age\": 56}]";
  struct person { char *name; int age; } people[2];

  json_t json_obj, json, json_obj1;
  json_load(&json_obj, json_data_obj);
  json_load(&json_obj1, json_data_obj1);
  json_load(&json, json_data);

  size_t length;
  char *name = json_get_string(json_obj.root, "name");
  int age = (int)json_get_number(json_obj.root, "age");
  char *name1 = json_get_string(json_get_object(json_obj1.root, "dat"), "name");
  int age1 = (int)json_get_number(json_get_object(json_obj1.root, "dat"), "age");
  log_info("\n[JSON object1] > \n  |  name:%s\n  |  age: %d\n  |", name1, age1);
  log_info("\n[JSON object] > \n  |  name:%s\n  |  age: %d\n  |", name, age);
  int qty = 0;
  json_object_t **objects = json_to_array(json.root, &length);
  for (size_t i = 0; i < length; ++i) {
      people[i].name = strdup(json_get_string(objects[i], "name"));
      people[i].age = (int)json_get_number(objects[i], "age");
      log_info("loaded #%d- %s (age %d)", i, people[i].name, people[i].age);
      qty++;
  }
  log_info("loaded %d People from %d Byte string", qty, strlen(json_data));


  json_unload(&json);
}
