<?php

  set_page_title(lang('time manager'));
  administration_tabbed_navigation(ADMINISTRATION_TAB_TIME);
  administration_crumbs(lang('time manager'));

  add_page_action(lang('unbilled time'), get_url('administration', 'time', array('status' => '0')));
  add_page_action(lang('billed time'), get_url('administration', 'time', array('status' => '1')));
  add_page_action(lang('view by user'), get_url('user', 'time'));
  add_page_action(lang('view by project'), get_url('project', 'time'));

?>

<h2><?php echo lang('view time by user'); ?></h2>

<?php if (isset($users) && is_array($users) && count($users)) { ?>
<table id="projects">
  <tr>
    <th></th>
    <th><?php echo lang('name'); ?></th>
    <th><?php echo lang('unbilled'); ?></th>
    <th><?php echo lang('billed'); ?></th>
  </tr>

<?php $counter = 0; ?>
<?php foreach ($users as $user) { ?>
  <tr>
    <td class="middle">
      &nbsp;<?php echo ++$counter; ?>.&nbsp;
    </td>
    <td class="long middle">
      &nbsp;<a href="<?php echo get_url('time', 'byuser', array('id' => $user->getId())); ?>"><?php echo clean($user->getDisplayName()) ?></a>
    </td>
    <td class="middle">
      &nbsp;<?php echo ProjectTimes::getTimeByUserStatus($user, 0, 'hours');?>&nbsp;<?php echo lang('hrs'); ?>
    </td>
    <td class="middle">
      &nbsp;<?php echo ProjectTimes::getTimeByUserStatus($user, 1, 'hours');?>&nbsp;<?php echo lang('hrs'); ?>
    </td>
  </tr> 
<?php } // foreach ?>
</table>
<?php } else { ?>
<?php echo lang('no users in company') ?>
<?php } // if ?>