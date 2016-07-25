<?php

function star_imgclean_drush_command() {
	$currentdate = date('Y_m_d_H_i_s');
	$items['star-clean'] = array(
		'description' => 'Clean the field of a content type',
		'aliases' => ['star-cln'],
		'callback' => 'star_imgclean',
		'arguments' => array(
			'content_type' => 'Content type machine name',
			'lang' => 'Language (en, und or fr)',
			'field_name' => 'Field to scan',
			),
	);
return $items;
}

function star_imgclean($content_type, $lang, $field_name) {
	// query for nodes newer than the specified date
	$query = $query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		->propertyCondition('type', $content_type);
	$result = $query->execute();

	// If there's results
	if (!empty($result)) {
		foreach ($result['node'] as $nid => $row) {
			echo "doing node " . $node->nid . " , " . $node->title . "..\r\n";

			// Load the node and assign the specified field to the content variable
			$node = node_load($row->nid, null, TRUE);
			$content = $node->{$field_name}[$lang][0]['value'];

			// Load the HTML from the field and parse the img src tags
			$doc = new DOMDocument();
			@$doc->loadHTML($content);
			$tags = $doc->getElementsByTagName('img');
			$count = 0;

			// If we found img tags
			if (count($tags) > 0) {
				foreach ($tags as $tag) {
					$count++;

					// Search for base64 encoded data within the img tag
					$verify = preg_match('/data:([^;]*);base64,(.*)/', $tag->getAttribute('src'), $match);
					if ($verify) {
						$data_raw = explode(',', $tag->getAttribute('src'));
						$data = base64_decode($data_raw[1]);
						$f = finfo_open();
						$mime_type = finfo_buffer($f, $data, FILEINFO_MIME_TYPE);
						
						// We need to make sure the encoding of the base64 data is actually an image
						$verify_mime = preg_match('/image\/(png|jpg|jpeg|gif)/', $mime_type, $mime_match);
						if ($verify_mime) {
							echo "doing image ". $count ." for node ". $node->nid . "..\r\n";
							$new_file = 'sites/default/files/'.$node->nid.'-imgclean-'.$count.'.jpg';
							$image = imagecreatefromstring($data);
							imagejpeg($image, $new_file, 50);
							
							// Update the img src with DOMDocument
							$tag->setAttribute('src', '/'.$new_file);
							var_dump($new_file);
							$content = $doc->saveHTML();
						}
					} else { 
						echo "skipping (no base64 data) .. \r\n";
					}
				}
	
				// Save the node
				$node->{$field_name}[$lang][0]['value'] = $content;
				node_save($node);
			} else {
				echo "skipping (no img tags) .. \r\n";
			}
                }
        } else {
		echo "No results found .. \r\n";
        }
        return;
}
