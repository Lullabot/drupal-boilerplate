# Example Starter Theme
It installs Susy and Breakpoint as an example.

#tl;dr
```bash
gem install bundler
bundle install
npm install -g grunt-cli
npm install
grunt compass
grunt watch
```

## Install Ruby
This task requires you to have Ruby, Sass, and Compass >=0.12.2 installed. If you're on OS X or Linux you probably already have Ruby installed; test with ```ruby -v``` in your terminal. You may need to have admin/root permissions to run the installation tasks.

## Install Bundler / Required Gems

```bash
gem install bundler
```

Bundler, [bundler](http://bundler.io/v1.3/gemfile.html), is the dependency manager for Ruby gems, like Sass modules. By using its `bundle` command, you can ensure that everyone on the team is using the same versions of the required Sass modules. (Required project and their versions are specified in the `Gemfile`.)

First, change directories to the `sites/all/themes/custom/yoursite` folder, then install all the required gems with:

```bash
bundle install
```

## Install Grunt
Grunt is a JavaScript task runner that helps automate things and is built on Node.js. In this project, it can be used to manage the SASS library dependencies as well as running the compass compilations. http://gruntjs.com/

You'll need Node.js and npm to install Grunt. Visit http://nodejs.org and click the big install button.

Then install grunt with:

```bash
npm install -g grunt-cli
```

Finally, change directories to the `sites/all/themes/lennon` folder and install the Node modules used in this project with:

```bash
npm install
```

## Available Grunt tasks
### Compile SASS
```bash
grunt compass
```

### Watch for changes
```bash
grunt watch
```

This task will automatically compile SASS when changes are deteched in the .scss files.

### LiveReload
Grunt watch can also be used with the [LiveReload](https://chrome.google.com/webstore/detail/livereload/jnihajbhpnppcggbcgedagnkighmdlei?hl=en) Chrome extension.

If you're using grunt watch + livereload, your browser will refresh when Grunt spots a change to your sass files.  That means no more punching the reload button in your browser for tiny changes during the theming process.

## Installing new Node.js modules
Either add to package.json or run

```bash
npm install <module> --save-dev
```

Then configure in Gruntfile.js

## Installing new Sass modules
Add a new line to the Gemfile and then (from the `sites/all/themes/lennon` theme folder), run:

```bash
bundle install
```

Then edit the theme's config.rb file to add a line saying `require "[module]"`.