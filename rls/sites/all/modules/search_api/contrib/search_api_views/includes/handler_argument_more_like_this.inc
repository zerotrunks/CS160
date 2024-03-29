<?php

/**
 * Views argument handler providing a list of related items for search servers
 * supporting the "search_api_mlt" feature.
 */
class SearchApiViewsHandlerArgumentMoreLikeThis extends SearchApiViewsHandlerArgument {

  /**
   * Specify the options this filter uses.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['fields'] = array('default' => array());
    return $options;
  }

  /**
   * Extend the options form a bit.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $index = search_api_index_load(substr($this->table, 17));
    if (!empty($index->options['fields'])) {
      $fields = array();
      foreach ($index->options['fields'] as $key => $field) {
        $fields[$key] = $field['name'];
      }
    }
    if (!empty($fields)) {
      $form['fields'] = array(
        '#type' => 'select',
        '#title' => t('Fields for Similarity'),
        '#description' => t('Select the fields that will be used for finding similar content. If no fields are selected, all available fields will be used.'),
        '#options' => $fields,
        '#size' => min(8, count($fields)),
        '#multiple' => TRUE,
        '#default_value' => $this->options['fields'],
      );
    }
    else {
      $form['fields'] = array(
        '#type' => 'value',
        '#value' => array(),
      );
    }
  }

  /**
   * Set up the query for this argument.
   *
   * The argument sent may be found at $this->argument.
   */
  public function query($group_by = FALSE) {
    $server = $this->query->getIndex()->server();
    if (!$server->supportsFeature('search_api_mlt')) {
      $class = search_api_get_service_info($server->class);
      throw new SearchApiException(t('The search service "!class" does not offer "More like this" functionality.',
          array('!class' => $class['name'])));
      return;
    }
    $fields = $this->options['fields'] ? $this->options['fields'] : array();
    if (empty($fields)) {
      foreach($this->query->getIndex()->options['fields'] as $key => $field) {
        $fields[] = $key;
      }
    }
    $mlt = array(
      'id' => $this->argument,
      'fields' => $fields,
    );
    $this->query->getSearchApiQuery()->setOption('search_api_mlt', $mlt);
  }
}
