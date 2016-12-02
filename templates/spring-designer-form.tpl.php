<?php
$labels = array();
foreach ($form as $key => $form_item) {
	if (is_array($form_item) && isset($form_item['#title'])) {
		$labels[$key] = $form_item['#title'];
		unset($form[$key]['#title']);
	}
}
?>
	<div class="row">
		<div class="col-lg-2 col-md-2 col-sm-6 col-xs-6 spring-designer-form-item spring-designer-form-units">
			<label><?php echo $labels['spring_designer_units'] ?></label>
			<?php print render($form['spring_designer_units']) ?>
		</div>
		<div class="col-lg-3 col-md-3 col-sm-6 col-xs-6 spring-designer-form-item spring-designer-form-material">
			<label><?php echo $labels['spring_designer_material'] ?></label>
			<?php print render($form['spring_designer_material']) ?>
		</div>
		<div class="col-lg-3 col-md-3 col-sm-6 col-xs-6 spring-designer-form-item spring-designer-form-diameter">
				<label><?php echo $labels['spring_designer_diameter_type'] ?></label>
			<div class="row">				
				<div class="col-xs-7 spring-designer-form-diameter-type"><?php print render($form['spring_designer_diameter_type']) ?></div>
				<div class="col-xs-5 spring-designer-form-diameter-value"><?php print render($form['spring_designer_diameter_value']) ?></div>
			</div>			
		</div>
		<div class="col-lg-2 col-md-2 col-sm-6 col-xs-6 spring-designer-form-item spring-designer-form-freelength">
			<label><?php echo $labels['spring_designer_free_length'] ?></label>
			<?php print render($form['spring_designer_free_length']) ?>
		</div>
		<div class="col-lg-2 col-md-2 col-sm-6 col-xs-12 spring-designer-form-item spring-designer-form-submit">
			<label>&nbsp;</label>
			<?php print render($form['submit']) ?>
		</div>
		
		<div class="col-xs-12 spring-designer-form-table-wrapper">
			<?php print render($form['spring_designer_table_wrapper']) ?>
		</div>
	</div>
<?php
print drupal_render_children($form);
