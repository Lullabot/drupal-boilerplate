<div id="yoursite-shiny__node-edit">
  <div class="yoursite-shiny__node-edit__main">
    <?php print drupal_render_children($form)?>
  </div>
  <aside class="yoursite-shiny__node-edit__sidebar">
    <div class="yoursite-shiny__node-edit__additional-settings">
      <?php print render($additional_settings); ?>
    </div>
    <div class="yoursite-shiny__node-edit__buttons">
      <?php print render($buttons); ?>
    </div>
  </aside>
</div>