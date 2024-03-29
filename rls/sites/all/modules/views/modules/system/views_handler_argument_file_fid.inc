<?php
/**
 * Argument handler to accept a file id.
 */
class views_handler_argument_file_fid extends views_handler_argument {
  /**
   * Override the behavior of title(). Get the title of the file.
   */
  function title() {
    $title = db_select('file_managed', 'f')
      ->fields('f', array('filename'))
      ->condition('fid', $this->argument)
      ->execute()
      ->fetchField();
    if (empty($title)) {
      return t('No title');
    }

    return check_plain($title);
  }
}
