/**
 * Homepage tests.
 *
 * These tests make some assertions on a Drupal vanilla installation.
 * Feel free to change them to suit your needs.
 */

casper.test.begin('Tests homepage structure', 3, function suite(test) {
  // Open the homepage.
  casper.start();
  casper.thenOpen('/', function() {
    // Check that we get a 200 response code.
    test.assertHttpStatus(200, 'Homepage was loaded successfully.');
    // Check the presence of the main items in the page.
    test.assertExists('a#logo', 'Header link to the homepage is present.');
    test.assertExists('form#user-login-form', 'Login form is present.');
  });

  casper.run(function() {
    test.done();
  });
});
