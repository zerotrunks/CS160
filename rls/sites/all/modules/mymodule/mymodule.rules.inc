<?php
/**
* Implementation of hook_rules_action_info().
*/
function mymodule_rules_action_info() {
  return array(
    'mymodule_action_mail_to_users_of_group' => array(
      'label' => t('Send an email to all users of a group'),
      'module' => 'MyModule',
      'eval input' => array('subject', 'message', 'from', 'group'),
    ),
  );
}
?>
