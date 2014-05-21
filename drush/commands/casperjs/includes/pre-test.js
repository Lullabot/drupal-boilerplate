/**
 * Creates sessions for all users.
 */
casper.test.begin("Creating sessions for all users.", function suite(test) {
  casper.start();

  casper.then(function() {
    casper.each(casper.boilerUsers, function(self, user) {
      casper.boilerCreateSession(user);
    });
  });

  casper.thenOpen('user/logout', function() {
    test.assertHttpStatus(403, 'No session is active.');
  });

  casper.run(function () {
    test.done();
  });
});
