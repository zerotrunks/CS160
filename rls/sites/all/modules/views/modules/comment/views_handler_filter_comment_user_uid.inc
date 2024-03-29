<?php

/**
 * Filter handler to accept a user id to check for nodes that user posted or
 * commented on.
 */
class views_handler_filter_comment_user_uid extends views_handler_filter_user_name {
  function query() {
    $this->ensure_my_table();

    $subselect = db_select('comment', 'c');
    $subselect->addField('c', 'cid');
    $subselect->condition('c.uid', $this->value, $this->operator);
    $subselect->where("nid = $this->table_alias.nid");

    $condition = db_or()
      ->condition("$this->table_alias.uid", $this->value, $this->operator)
      ->exists($subselect);

    $this->query->add_where(0, $condition);
  }
}
