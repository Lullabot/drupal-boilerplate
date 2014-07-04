module.exports = function(grunt) {

  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    compass: {
      dist: {
        options: {
          bundleExec: true,
          config: 'config.rb'
        }
      }
    },
    watch: {
      options: {
        livereload: true
      },
      css: {
        files: ['scss/**/*.scss'],
        tasks: ['compass'],
        options: {
          spawn: false
        }
      }
    }
  });

  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-contrib-compass');

  grunt.registerTask('default', ['watch', 'compass']);

};
