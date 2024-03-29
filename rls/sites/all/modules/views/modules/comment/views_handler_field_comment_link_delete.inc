<?php
/**
 * Field handler to present a link to delete a node.
 */
class views_handler_field_comment_link_delete extends views_handler_field_comment_link {
  function access() {
    //needs permission to administer comments in general
    return user_access('administer comments');
  }

  function render_link($data, $values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $cid =  $this->get_value($values, 'cid');

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "comment/" . $cid . "/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    return $text;
  }
}
