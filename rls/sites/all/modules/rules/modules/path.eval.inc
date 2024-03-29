<?php

/**
 * @file
 * Contains rules integration for the path module needed during evaluation.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Action implementation: Path alias.
 */
function rules_action_path_alias($source, $alias, $langcode = LANGUAGE_NONE) {
  $alias = rules_clean_path($alias);
  if (!$alias) {
    path_delete(array('source' => $source, 'language' => $langcode));
  }
  elseif (!$source) {
    path_delete(array('alias' => $alias, 'language' => $langcode));
  }
  // Only set the alias if the alias is not taken yet.
  elseif (!path_load(array('alias' => $alias, 'language' => $langcode))) {
    // Update the existing path or create a new one.
    if ($path = path_load(array('source' => $source, 'language' => $langcode))) {
      $path['alias'] = $alias;
    }
    else {
      $path = array('source' => $source, 'alias' => $alias, 'language' => $langcode);
    }
    path_save($path);
  }
  else {
    rules_log('The configured alias %alias already exists. Aborting.', array('%alias' => $alias));
  }
}

/**
 * Action Implementation: Set the URL alias for a node.
 */
function rules_action_node_path_alias($node, $alias) {
  $alias = rules_clean_path($alias);
  $langcode = isset($node->language) ? $node->language : LANGUAGE_NONE;
  // Only set the alias if the alias is not taken yet.
  if (($path = path_load(array('alias' => $alias, 'language' => $langcode))) && (empty($node->path['pid']) || $node->path['pid'] != $path['pid'])) {
    rules_log('The configured alias %alias already exists. Aborting.', array('%alias' => $alias));
    return FALSE;
  }
  $node->path['alias'] = $alias;
}

/**
 * Action Implementation: Set the URL alias for a node.
 */
function rules_action_taxonomy_term_path_alias($term, $alias) {
  $alias = rules_clean_path($alias);
  // Only set the alias if the alias is not taken yet.
  if (($path = path_load(array('alias' => $alias, 'language' => LANGUAGE_NONE))) && (empty($term->path['pid']) || $term->path['pid'] != $path['pid'])) {
    rules_log('The configured alias %alias already exists. Aborting.', array('%alias' => $alias));
    return FALSE;
  }
  $term->path['alias'] = $alias;
}

/**
 * Condition implementation: Check if the path has an alias.
 */
function rules_condition_path_has_alias($source, $langcode = LANGUAGE_NONE) {
  return (bool) drupal_lookup_path('alias', $source, $langcode);
}

/**
 * Condition implementation: Check if the URL alias exists.
 */
function rules_condition_path_alias_exists($alias, $langcode = LANGUAGE_NONE) {
  $alias = rules_clean_path($alias);
  return (bool) drupal_lookup_path('source', $alias, $langcode);
}

/**
 * Cleans the given path using the configured path cleaning method.
 */
function rules_clean_path($path) {
  // @todo Add a setting for choosing path cleaning callbacks.
  $function = variable_get('rules_path_cleaning_callback', 'rules_path_default_cleaning_method');
  return $function($path);
}

/**
 * Cleans the given path by replacing non ASCII characters with the replacment character.
 *
 * Path cleaning may be adapted by overriding the configuration variables
 * @code rules_clean_path @endcode,
 * @code rules_path_replacement_char @endcode and
 * @code rules_path_transliteration @endcode
 * in the site's settings.php file.
 */
function rules_path_default_cleaning_method($path) {
  $replace = variable_get('rules_path_replacement_char', '-');
  if ($replace) {
    // If the transliteration module is enabled, transliterate the alias first.
    if (module_exists('transliteration') && variable_get('rules_path_transliteration', TRUE)) {
      $path = transliteration_get($path);
    }

    $array = variable_get('rules_clean_path', array('/[^a-zA-Z0-9\/\-_]+/', $replace));
    $array[2] = $path;
    // Replace it and remove trailing and leading replacement characters.
    return trim(call_user_func_array('preg_replace', $array), $replace);
  }
  else {
    return $path;
  }
}

/**
 * @}
 */
