<?php

/**
 * Field handler to provide simple renderer that allows linking to a taxonomy
 * term.
 */
class views_handler_field_taxonomy extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   *
   * This constructer assumes the taxonomy_term_data table. If using another
   * table, we'll need to be more specific.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['vid'] = 'vid';
    $this->additional_fields['tid'] = 'tid';
    $this->additional_fields['vocabulary_machine_name'] = array(
      'table' => 'taxonomy_vocabulary',
      'field' => 'machine_name',
    );
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_taxonomy'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to taxonomy option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this field to its taxonomy term page'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the taxonomy.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    $tid = $this->get_value($values, 'tid');
    if (!empty($this->options['link_to_taxonomy']) && !empty($tid) && $data !== NULL && $data !== '') {
      $term = new stdClass();
      $term->tid = $tid;
      $term->vid = $this->get_value($values, 'vid');
      $term->vocabulary_machine_name = $values->{$this->aliases['vocabulary_machine_name']};
      $this->options['alter']['make_link'] = TRUE;
      $uri = entity_uri('taxonomy_term', $term);
      $this->options['alter']['path'] = $uri['path'];
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
