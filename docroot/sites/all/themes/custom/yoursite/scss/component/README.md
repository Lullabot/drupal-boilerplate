# Component Partials

Components make up the core of our modular architecture.

The core concept of components is that they are location independent within the
theme. Moving a component from the sidebar to the footer shouldn't have any
effect on that component's styling.

There will, of course, be exceptions to this rule based on whether or not the
design calls for variants in a component based on placement.

For example:

## A "Next Event" component.

```html
<section class="next-event">
  <h2 class="next-event__title">Secret Meetings</h2>
  <h3 class="next-event__day">Monday</h3>
</section>
```

Rather than adding styles to `.next-event h2`, style the class. Following the
CSS coding standards for Drupal, we (typically) won't need to nest or Sass.

```css
.next-event {
  color: pink;
}
.next-event__title {
  font-size: 9000px;
}
.next-event__day {
  border-top: 1px solid green;
}
```

The beauty of styling classes rather than sub-selectors is that unclassed `h2`
and `h3` elements will look the same if we need to add them into the component's
markup - the styles for the markup are contained.

## Typical contents

Filename              | Purpose
--------------------- | ---------------------------------------------
`_components.scss`    | Used to import other files in this directory
`_your-component.scss`| Whatever you can abstract from the designs!
