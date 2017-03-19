# Global Partials

A collection of partials that do not get compiled directly
that are used as assets for Sass.

A general rule-of-thumb is if a Sass partials contents do not actually print
something into the compiled CSS, it belongs here.

## Typical contents

Filename          | Purpose
----------------- | ---------------------------------------------
`_global.scss`    | Used to import other files in this directory
`_fonts.scss`     | Sets up fonts and icon fonts
`_variables.scss` | Contains variables used across the project
`_mixins.scss`    | Contains Sass mixins for the project

