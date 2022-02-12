
void on_progress(progress_data_t *data);


int progress_demo(void) {
  progress_t *progress = progress_new(100, 60);

  progress->fmt = "progress [:bar] :percent :elapsed";

  // listen for progress
  progress_on(progress, PROGRESS_EVENT_PROGRESS, on_progress);

  // tick progress
  progress_tick(progress, 10);
}


void on_progress(progress_data_t *data) {
  progress_write(data->holder);
}
