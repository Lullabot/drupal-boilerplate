/**
 * Tests creating an article and viewing it.
 *
 * Requires the admin user account to be set at common.js in order
 * to log in.
 */

casper.test.begin("Test article workflow.", 5, function suite(test) {

  var sampleText = "Sample Article " + new Date().getTime();
  var node_view_expression = /node\/([0-9]+)/i;
  var nid = 0;
  var node_links = [];
  var date = '';
  var time = '';

  // Returns administrative links within a node edit form.
  function nodeLinks() {
    var links = [];
    jQuery('.tabs.primary a').each(function () {
      links.push(jQuery(this).attr('href'));
    });
    return links;
  }

  // Create an unpublished article.
  casper.boilerStartAs('admin');
  casper.thenOpen('node/add/article', function() {
    test.assertHttpStatus(200, "Authenticated user can access the Create article form.");
    var params = {
      'title' : sampleText,
      'body[und][0][value]': 'The full text of ' + sampleText + '.'
    };
    this.fill('.node-form', params, false);
  });
  casper.thenEvaluate(function () {
    jQuery('input[name="status"]').trigger('click');
  });
  casper.thenClick('#edit-submit');

  // Confirm article is created and capture nid.
  casper.waitForUrl(node_view_expression, function() {
    var matches = node_view_expression.exec(this.getCurrentUrl());
    test.assertTruthy(matches[1], "Successfully created new article node: (nid " + matches[1] + ")");
    nid = matches[1];

    // Get the paths to this article.
    node_links = node_links.concat(this.evaluate(nodeLinks));
    view = node_links[0];
    view = view.slice(1);
    this.echo("The new article is viewable at the path " + view + ".");

    form = node_links[1];
    form = form.slice(1);
    this.echo("The new article is edited at the path " + form + ".");

    // View the article as editor.
    casper.thenOpen(view, function() {
      test.assertTextExists(sampleText, "The new article " + sampleText + " is viewable to editor.");
    });

    // Check that an anonymous user can't see an unpublished article.
    casper.boilerEndSession();
    casper.thenOpen(view, function() {
      test.assertHttpStatus(403, "Anonymous users can't view unpublished content");
    });

    // Delete the article when testing is done.
    casper.boilerBeginSession('admin');
    casper.thenOpen('node/' + nid + '/delete');
    casper.thenClick('#edit-submit');
    casper.then(function () {
      test.assertTextExists('has been deleted', "Sample article was deleted");
    });
    casper.boilerEndSession();

  });

  casper.boilerRun();
});
