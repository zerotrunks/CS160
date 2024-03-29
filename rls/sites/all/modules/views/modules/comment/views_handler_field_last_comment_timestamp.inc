<?php

class views_handler_field_last_comment_timestamp extends views_handler_field_date {
  function construct() {
    parent::construct();
    $this->additional_fields['comment_count'] = 'comment_count';
  }

  function render($values) {
    $comment_count = $this->get_value($values, 'comment_count');
    if (empty($this->options['empty_zero']) || $comment_count) {
      return parent::render($values);
    }
    else {
      return NULL;
    }
  }
}
