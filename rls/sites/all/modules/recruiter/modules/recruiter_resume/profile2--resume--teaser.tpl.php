<?php
// $Id$

/**
 * @file
 * Default theme implementation for profiles.
 *
 * Available variables:
 * - $content: An array of comment items. Use render($content) to print them all, or
 *   print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $title: The (sanitized) profile type label.
 * - $url: The URL to view the current profile.
 * - $page: TRUE if this is the main view page $url points too.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. By default the following classes are available, where
 *   the parts enclosed by {} are replaced by the appropriate values:
 *   - entity-profile
 *   - profile-{TYPE}
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>>
        <a href="<?php print $url; ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>
  <div class="content"<?php print $content_attributes; ?>>
    <div class="resume-teaser-container clearfix">
    <?php 
      $field_collection_personal_info = reset($content['field_resume_personal_info']['0']['entity']['field_collection_item']);
      if (!empty($field_collection_personal_info['field_resume_photo'])): ?>
        <div class="resume-teaser-photo">
          <?php print render($field_collection_personal_info['field_resume_photo']); ?>
        </div>
    <?php endif; ?>
    
    <?php
      print render($content['field_resume_job_preferences']);
    ?>
    </div>
  </div>
</div>
