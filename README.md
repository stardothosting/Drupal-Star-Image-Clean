# Drupal Star Image Clean
## Clean up base64 encoded images in Drupal content fields
This is a drupal module that establishes a drush command that will iterate all nodes of the specified content type's fields to seek and convert base64 encoded image data to files. The node is then re-saved with a proper image tag that links to the file instead of raw base64 data.

The reason why base64 data for an image is bad is because it gets saved into the Drupal database. Speed, usability and availability are directly affected if too much base64 data is saved into the database directly. This is especailly true if its uncompressed image data.

How this happens in the first place is because the TinyMCE WYSIWYG editor allows end-users to drag and drop images into the editor. It converts them to an image and makes for a really easy way for the user to inject images and medai into their content. What they dont see is that the image is base64 data and not actually saved as a file.

This module was created in conjunction with the [Toronto based web desgin](https://www.shift8web.ca) and development company, Shift8.

## Drush command reference

The drush command takes 3 arguments :

####content type
####lang
####field

An example commmand would be :

```drush star-clean page en body```

Where "page" is the content type name, "en" is the language (if you dont use multi-language, just specify "und") and "body" is the field that needs to be cleaned up.

## Full technical breakdown

You can read our blog post that walks through all parts of this module here : https://www.shift8web.ca/blog/2016/07/speed-drupal-clean-base64-encoded-images/
