# Automated testing

This directory contains [CasperJS](http://casperjs.org) tests that ensure the
stability of the site.

## Installation

### Requirements

* Python 2.6 or greater.
* PhantomJS 1.9.2.
* CasperJS 1.1-beta3.

### OSX - Homebrew
Homebrew will install the phantomjs dependency automatically.

```bash
brew install casperjs --devel
```

### Manual installation

#### Python

Python is already installed in most platforms. Check the pyhon version with the
following command:

```bash
python --version
```

If the above output is less than 2.6, go to https://www.python.org/download,
find the link for your platform at the 2.7 section (even though CasperJS seems
to work on Python 3 we have not tested it yet).

You can find further installation tips at
http://docs.casperjs.org/en/latest/installation.html#prerequisites

#### CasperJS

```bash
cd /usr/share
sudo git clone git://github.com/n1k0/casperjs.git
cd casperjs
git checkout 1.1-beta3
sudo ln -sf `pwd`/bin/casperjs /usr/local/bin/casperjs
```

#### PhantomJS

Download phantomjs 1.9.2 http://phantomjs.org/download.html to `/usr/share`

```bash
cd /usr/share/phantomjs-1.9.2-linux-x86_64/bin
sudo ln -sf `pwd`/phantomjs /usr/local/bin/phantomjs
```

### Verify your installation

When running `casperjs`, the output should be:

```bash
casperjs
CasperJS version 1.1.0-beta3 at /usr/share/casperjs, using phantomjs version 1.9.2
```

## Running tests
Assuming your local environment is set up at http://drupal.local, all tests may
be run with the following command:

```bash
cd path-to-drupal-root
drush casperjs --url=http://drupal.local
```

You can also run a specific test by giving it as an argument to the command:

```bash
drush casperjs --url=http://drupal.local
```

*NOTE* `drush casperjs` is a wrapper for `casperjs` which sets some useful defaults
when running tests for your Drupal site. Run `drush casperjs -h` for a list of all the
available options and arguments.

## Writing tests

Tests are JavaScript files which are located at the `tests` directory.
They can be organized either by which aspect of the site they test (the homepage,
the contact form, the editorial workflow) or by mapping its name with a module
so it tests the functionality added by that particular module. The choice is
yours to take.

In order to write a new test, copy any of the existing ones and use it as a
template. Be sure to read through the includes such as `drush/commands/casperjs/includes/common.js`
and drush/commands/casperjs/includes/session.js to find if there are any helper
functions you should be aware of in addition to the ones provided by CasperJS.

Some useful resources for writing tests are:
  * [Navigation steps](http://docs.casperjs.org/en/latest/faq.html#how-does-then-and-the-step-stack-work)
    let you wait for certain events such a page getting fully rendered before
    running assertions over it.
  * The [casper](http://docs.casperjs.org/en/latest/modules/casper.html) object has
    commands to interact with the browser such as opening a URL or filling
    out a form.
  * The [test](http://docs.casperjs.org/en/latest/modules/tester.html)
    object contains methods to run assertions over the current context.
  * `drush/commands/casperjs/includes/common.js` has a list of useful methods to
    navigate through a Drupal project. Rely on these methods as much as possible
    in order to simplify your testing code.
  * `drush/commands/casperjs/includes/session.js` implements session management so
    you can swap from an anonymous user to an authentcated user in just one step.

## Cookies
Some tests like will need an authenticated user to work with order to test the
backend. The `drush casperjs` command creates a temporary cookies file at the
temorary directory while running tests to store cookie information and deletes it
the next time tests are run.

If you need session management in your test call `casper.boilerRun()` to run
your test so that all opened sessions are closed for the next test run.

## Tips
### Taking screenshots
You can take a screenshot and capture markup with `casper.boilerCapture('my-unique-string');`.
This is perfectly fine to commit to your test suite, as this will do nothing
unless you specifically configured your environment to actually save these files.
There are three ways to do this:

The first is to turn on capturing by exporting
an environment variable:

```bash
export BOILER_TEST_CAPTURE=true
```

The second is to run the `drush casperjs` command with debug mode turned on.
See the *Debugging* section for more details on that.

The third method is to pass `true` as the second argument to `casper.boilerCapture()`.
This will force capturing. Do *not* commit code in this state, as this means
capturing will occur on all environments.

Once enabled, this command will save a screenshot to `./screenshots/my-unique-string-12345678.jpg`
and the HTML markup to `./pages/my-unique-string-1234578.html`, where `12345678`
is the current timestamp. You may need to create those directories yourself and
give proper write permissions.

Alternatively, you can use `casper.captureSelector('filename', 'div.some-class');`
to take a screenshot of a given selector. In this case, specify a full filename
to save the capture to, such as `screenshots/my-selector-12345678.jpg`. You can
find more examples about capturing [here](http://docs.casperjs.org/en/latest/modules/casper.html#capture).

### Debugging

If you would like to see a more verbose output of the test suites, you can run
the `drush casperjs` command with the `-d` flag:

```bash
drush casperjs -d
```

This will execute CasperJS with `log-level` set to `debug`. In your scripts, you
can log to this debug mode by using `casper.log('my message', 'debug');`.

In addition, other helpful debugging tools are [`casper.debugPage()`](http://casperjs.readthedocs.org/en/latest/modules/casper.html#debugpage)
and [`casper.debugHTML()`](http://casperjs.readthedocs.org/en/latest/modules/casper.html#debughtml).

### Evaluating code

The [casper.evaluate()](http://docs.casperjs.org/en/latest/modules/casper.html#evaluate)
method (and its alternatives such as `casper.evaluateOrDie()`, `casper.thenEvaluate()` or
`test.assertEvaluate()`) are highly powerful since they will run JavaScript code
on the page just as if you were debugging with the browser's JavaScript console.
