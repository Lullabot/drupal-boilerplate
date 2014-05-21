/**
 * Helper methods for navigating through a Drupal site.
 *
 * This file is included automatically by the casperjs Drush command.
 */
var utils = require('utils');
var f     = utils.format;

// Set the default timeout to 2 minutes, since the backend experience can be
// quite slow. If you change this value in a test suite, please set it back.
casper.options.waitTimeout = 120000;

/**
 * Run a test suite, ending all sessions when done.
 */
casper.boilerRun = function(time) {
  casper.run(function(self) {
    casper.boilerEndSession();
    self.test.done();
  }, time);
};

/**
 * Listen to the open.location event in order to prepend the hostname.
 *
 * This filter will automatically prepend the full hostname that you are running
 * tests against to a given path. For example, if you run
 * casper.thenOpen('node/1'), it will convert it to
 * http://myhostname/node/1
 */
casper.setFilter('open.location', function(location) {
  if (utils.isUndefined(location)) {
    location = "";
  }
  var cleanPath = location.replace(/^\//, '');
  return casper.cli.get('url') + '/' + cleanPath;
});

/**
 * Set the viewport to a different breakpoint.
 *
 * @param string $size
 *   A breakpoint name. One of mobile, tablet, tablet-landscape or desktop.
 */
casper.thenChangeViewport = function (size) {
  this.then(function () {
    if (size === 'mobile') {
      this.viewport(320, 400);
    } else if (size === 'tablet') {
      this.viewport(768, 1024);
    } else if (size === 'tablet-landscape') {
      this.viewport(1020, 1020);
    } else if (size === 'desktop') {
      this.viewport(1280, 1280);
    } else {
      test.fail('Responsive Check Not Properly Defined')
    }
  });
};

/**
 * Save page markup to a file. Respect an existing savePageContent function, if
 * casper.js core introduces one.
 *
 * @param String targetFile
 *   A target filename.
 * @return Casper
 */
casper.savePageContent = casper.savePageContent || function(targetFile) {
  var fs = require('fs');

  // Get the absolute path.
  targetFile = fs.absolute(targetFile);
  // Let other code modify the path.
  targetFile = this.filter('page.target_filename', targetFile) || targetFile;
  this.log(f("Saving page html to %s", targetFile), "debug");
  // Try saving the file.
  try {
    fs.write(targetFile, this.getPageContent(), 'w');
  } catch(err) {
    this.log(f("Failed to save page html to %s; please check permissions", targetFile), "error");
    this.log(err, "debug");
    return this;
  }

  this.log(f("Page html saved to %s", targetFile), "info");
  // Trigger the page.saved event.
  this.emit('page.saved', targetFile);

  return this;
};

/**
 * Capture the markup and screenshot of the page. NOTE: Capturing will only
 * occur in one of two ways: either you pass true as the second argument to this
 * function (not recommended, except for testing purposes) or if you have set
 * the BOILER_TEST_CAPTURE environment variable to true, like so:
 *
 * $ export BOILER_TEST_CAPTURE=true
 *
 * @param string filename
 *   The name of the file to save, without the extension.
 * @param boolean force
 *   Force capturing of screenshots and markup.
 */
casper.boilerCapture = function(filename, force) {
  // If we are not capturing, simply return.
  if (!this.boilerVariableGet('BOILER_TEST_CAPTURE', false) && !force) {
    return;
  }
  // If we didn't get a filename, use an empty string.
  if (utils.isFalsy(filename)) {
    filename = '';
  }
  // Otherwise, add a dash delimiter to the end.
  else {
    filename += '-';
  }
  // Make the filename unique with a timestamp.
  filename += new Date().getTime();
  var screenshot = 'screenshots/' + filename + '.jpg',
      markup = 'pages/' + filename + '.html',
      prefix = '',
      screenshot_url = screenshot,
      markup_url = markup;
  // If we have a Drupal files directory available, use it.
  if (casper.boilerVariableGet('BOILER_FILES_DIRECTORY')) {
    prefix = casper.boilerVariableGet('BOILER_FILES_DIRECTORY') + '/testing/';
    screenshot_url = casper.boilerVariableGet('BOILER_FILES_URL') + '/' + screenshot;
    markup_url = casper.boilerVariableGet('BOILER_FILES_URL') + '/' + markup;
  }
  this.capture(prefix + screenshot);
  this.test.comment(f('Saved screenshot to %s.', screenshot_url));
  this.savePageContent(prefix + markup);
  this.test.comment(f('Saved markup to %s.', markup_url));
};

/**
 * Some event listeners to log errors to boilerCapture.
 */
casper.on('error', function() {
  casper.boilerCapture('error');
});
casper.on('step.error', function() {
  casper.boilerCapture('step-error');
});
casper.on('step.timeout', function() {
  casper.boilerCapture('step-timeout');
});
casper.on('waitFor.timeout', function() {
  casper.boilerCapture('wait-for-timeout');
});
casper.on('started', function() {
  // If we have http authentication credentials, use them.
  if (casper.boilerVariableGet('BOILER_TEST_HTTP_USERNAME') && casper.boilerVariableGet('BOILER_TEST_HTTP_PASSWORD')) {
    this.log("Using HTTP Authentication.");
    casper.setHttpAuth(casper.boilerVariableGet('BOILER_TEST_HTTP_USERNAME'), casper.boilerVariableGet('BOILER_TEST_HTTP_PASSWORD'));
  }
});

/**
 * Retrieves an environment variable.
 *
 * @param String key
 *   The name of the variable to retrieve.
 * @param defaultValue
 *   The default value to return if no environment variable exists.
 * @return
 *   The value from the environment variable, or the default if none was found,
 *   or undefined if neither are found.
 */
casper.boilerVariableGet = function(key, defaultValue) {
  var variables = require('system').env;
  return utils.isUndefined(variables[key]) ? defaultValue : variables[key];
};

// Comment about the status of the BOILER_TEST_CAPTURE variable.
if (casper.boilerVariableGet('BOILER_TEST_CAPTURE')) {
  casper.test.comment('Capturing of screenshots and markup is enabled.');
}
