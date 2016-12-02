<?php

/**
 * @file
 * Administrative Form for the Spring Designer module.
 */


function spring_designer_admin_form($form, &$form_state) {

  $form = array();

  $form['spring_designer_admin_fixed_variables'] = array(
    '#type' => 'fieldset',
    '#title' => "Fixed Variables",
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
    );

  $form['spring_designer_admin_fixed_variables']['spring_designer_admin_wire_sizes'] = array(
    '#type' => 'textarea',
    '#title' => 'Wire Range',
    '#rows' => 24,
    '#default_value' => variable_get('spring_designer_admin_wire_sizes', ''),
    '#description' => 'Enter the list of all the wire sizes available, one each line in the format "Reference,Size(mm),Size(in),Lower Tensile (carbon),Lower Tensile (stainless)"',
  );

  $form['spring_designer_admin_fixed_variables']['spring_designer_admin_solid_stress'] = array(
    '#type' => 'textarea',
    '#title' => 'Stress Rating',
    '#rows' => 6,
    '#default_value' => variable_get('spring_designer_admin_solid_stress', ''),
    '#description' => 'Enter the list of all the Stress ratings available, one each line in the format "name = value"',
  );


  // Value Limits
  $form['spring_designer_admin_limits'] = array(
    '#type' => 'fieldset',
    '#title' => "Value Limits",
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
    );

  $form['spring_designer_admin_limits']['spring_designer_od_min'] = array(
    '#type' => 'textfield',
    '#title' => "Min outside diameter",
    '#size' => 6,
    '#default_value' => variable_get('spring_designer_od_min', 2),
    '#attributes' => array('maxlength' => 4, 'size' => 4), 
  );

  $form['spring_designer_admin_limits']['spring_designer_od_max'] = array(
    '#type' => 'textfield',
    '#title' => "Max outside diameter",
    '#size' => 6,
    '#default_value' => variable_get('spring_designer_od_max', 49.9),
    '#attributes' => array('maxlength' => 4, 'size' => 4), 
  );

  $form['spring_designer_admin_limits']['spring_designer_fl_max'] = array(
    '#type' => 'textfield',
    '#title' => "Max Free Length",
    '#size' => 6,
    '#default_value' => variable_get('spring_designer_fl_max', 1000),
    '#attributes' => array('maxlength' => 4, 'size' => 4), 
  );

  $form['spring_designer_admin_limits']['spring_designer_compression_fl_od_min'] = array(
    '#type' => 'textfield',
    '#title' => "Min Compression Free Length / Outside diameter",
    '#size' => 6,
    '#default_value' => variable_get('spring_designer_compression_fl_od_min', 1),
    '#attributes' => array('maxlength' => 4, 'size' => 4), 
  );

  $form['spring_designer_admin_limits']['spring_designer_compression_fl_od_max'] = array(
    '#type' => 'textfield',
    '#title' => "Max Compression Free Length / Outside diameter",
    '#size' => 6,
    '#default_value' => variable_get('spring_designer_compression_fl_od_max', 9.9),
    '#attributes' => array('maxlength' => 4, 'size' => 4), 
  );

  $form['spring_designer_admin_limits']['spring_designer_extension_fl_od_min'] = array(
    '#type' => 'textfield',
    '#title' => "Min Extension Free Length / Outside diameter",
    '#size' => 6,
    '#default_value' => variable_get('spring_designer_extension_fl_od_min', 3),
    '#attributes' => array('maxlength' => 4, 'size' => 4), 
  );

  $form['spring_designer_admin_limits']['spring_designer_extension_fl_od_max'] = array(
    '#type' => 'textfield',
    '#title' => "Max Extension Free Length / Outside diameter",
    '#size' => 6,
    '#default_value' => variable_get('spring_designer_extension_fl_od_max', 20),
    '#attributes' => array('maxlength' => 4, 'size' => 4), 
  );


  // Error Messages
  $form['spring_designer_admin_errors'] = array(
    '#type' => 'fieldset',
    '#title' => "Error Messages",
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
    );

  $errors = variable_get('spring_designer_errors');

  $form['spring_designer_admin_errors']['spring_designer_empty_result'] = array(
  '#type' => 'textarea',
  '#title' => "Empty Result",
  '#rows' => 3,
  '#default_value' => variable_get('spring_designer_empty_result', 'Your search has generated 0 results'),
  );

  $form['spring_designer_admin_errors']['spring_designer_error_1'] = array(
  '#type' => 'textfield',
  '#title' => "Error Message 01",
  '#size' => 128,
  '#default_value' => isset($errors['error_1']) ? $errors['error_1'] : '',
  );

  $form['spring_designer_admin_errors']['spring_designer_error_2'] = array(
  '#type' => 'textfield',
  '#title' => "Error Message 02",
  '#size' => 128,
  '#default_value' => isset($errors['error_2']) ? $errors['error_2'] : '',
  );

  $form['spring_designer_admin_errors']['spring_designer_error_3'] = array(
  '#type' => 'textfield',
  '#title' => "Error Message 03",
  '#size' => 128,
  '#default_value' => isset($errors['error_3']) ? $errors['error_3'] : '',
  );

  $form['spring_designer_admin_errors']['spring_designer_error_4'] = array(
  '#type' => 'textfield',
  '#title' => "Error Message 04",
  '#size' => 128,
  '#default_value' => isset($errors['error_4']) ? $errors['error_4'] : '',
  );

  $form['spring_designer_admin_errors']['spring_designer_error_5'] = array(
  '#type' => 'textfield',
  '#title' => "Error Message 05",
  '#size' => 128,
  '#default_value' => isset($errors['error_5']) ? $errors['error_5'] : '',
  );

  $form['spring_designer_stress_rating_info'] = array(
  '#type' => 'text_format',
  '#title' => t('Stress Rating Info'),
  '#default_value' => variable_get('spring_designer_stress_rating_info', t('')),
  '#rows' => 9,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
  );

  return $form;

}


function spring_designer_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}


function spring_designer_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Save Fixed variables
  variable_set('spring_designer_admin_wire_sizes', $values['spring_designer_admin_wire_sizes']);
  variable_set('spring_designer_admin_solid_stress', $values['spring_designer_admin_solid_stress']);

  // Save Value Limits
  variable_set('spring_designer_od_min', $values['spring_designer_od_min']);
  variable_set('spring_designer_od_max', $values['spring_designer_od_max']);
  variable_set('spring_designer_fl_max', $values['spring_designer_fl_max']);
  variable_set('spring_designer_compression_fl_od_min', $values['spring_designer_compression_fl_od_min']);
  variable_set('spring_designer_compression_fl_od_max', $values['spring_designer_compression_fl_od_max']);
  variable_set('spring_designer_extension_fl_od_min', $values['spring_designer_extension_fl_od_min']);
  variable_set('spring_designer_extension_fl_od_max', $values['spring_designer_extension_fl_od_max']);

  // Save error Messages
  variable_set('spring_designer_empty_result', $values['spring_designer_empty_result']);
  
  $errors = array();
  foreach ($values as $key => $value) {
    $field = explode('_', $key);
    if ($field[0] = 'spring' && isset($field[2]) && $field[2] == 'error') {
      $ekey = $field[2] . '_' . $field[3];
      $errors[$ekey] = $value;
    }
  }
  variable_set('spring_designer_errors', $errors);
  variable_set('spring_designer_stress_rating_info', $values['spring_designer_stress_rating_info']['value']);

}