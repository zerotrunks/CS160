<!--Connect to database-->
<?php include("./phpcodes/dbconnect.php"); ?>
<?php 

$query = "SELECT node.title AS node_title, node.nid AS nid, node.created AS node_created," .
	" 'Fulfilled' AS field_data_field_letter_status_node_entity_type, GREATEST(node.changed, node_comment_statistics.last_comment_timestamp)" .
	" AS node_comment_statistics_last_updated FROM node" .
	" INNER JOIN field_data_field_letter_status ON node.nid = field_data_field_letter_status.entity_id" .
	" AND (field_data_field_letter_status.entity_type = 'node' AND field_data_field_letter_status.deleted = '0')" .
	" INNER JOIN node_comment_statistics ON node.nid = node_comment_statistics.nid" .
	" WHERE (( (node.status = '1') AND (node.type IN  ('recommendation_request'))" .
	" AND (field_data_field_letter_status.field_letter_status_tid = '426') ))";
//print($query);
$noncon_count = 0;
$noncon_dur_total = 0;
$noncon_avg_total = 0;
$result = mysql_query($query);
//check if current input already exist
if (!$result) 
{
    $message  = 'Error: ' . mysql_error() . "\n";
	die($message);
}
elseif ($result)
{

	print "Non-confidential requests";
	print "<br>\n";
	print "<table border='1'>";
	print "<tr>";
	print "<td><b>Title</b></td>";
	print "<td><b>Node ID</b></td>";
	print "<td><b>Post date</b></td>";
	print "<td><b>Status</b></td>";
	print "<td><b>Last update</b></td>";
	print "<td><b>Duration(Days)</b></td>";
	print "</tr>";
	//print "Title\tNode ID\tPost date\tStatus\tLast update\tduration";
	//print "<br>\n";
	while (list($title, $nid, $post, $status, $last_update) = mysql_fetch_array($result))
	{
		print "<tr>";

		//$duration = strtotime($post) - strtotime($last_update);
                //$duration = $duration/60/60/24;
		$post = date('Y-m-d', $post);
		$last_update = date('Y-m-d', $last_update);

                $diff = abs(strtotime($post) - strtotime($last_update));
                $years = floor($diff / (365*60*60*24));
                $months = floor(($diff - $years * 365*60*60*24) / (30*60*60*24));
                $duration = floor(($diff - $years * 365*60*60*24 - $months*30*60*60*24)/ (60*60*24));

		//$post = date('Y-m-d h:i', $post);
		//$last_update = date('Y-m-d h:i', $last_update);
		//print "$title, $nid, $post, $status, $last_update, $duration days";
		//print "<br>\n";
		print "<td>$title</td>";
		print "<td>$nid</td>";
		print "<td>$post</td>";
		print "<td>$status</td>";
		print "<td>$last_update</td>";
		print "<td>$duration</td>";
		$noncon_count++;
		$noncon_dur_total = $noncon_dur_total + $duration;
		print "</tr>";
	}
	print "</table>" ;
	
	$noncon_avg_total = $noncon_dur_total / $noncon_count;
	print "<b>Avg. duration to fulfilled a non-confidentila recomendation request = $noncon_avg_total</b>";
	print "<br>\n";
print "<br>\n";
}	
	$query = "SELECT node.title AS node_title, node.nid AS nid, node.created AS node_created, 'Fulfilled' AS field_data_field_status2_node_entity_type," .
	" GREATEST(node.changed, node_comment_statistics.last_comment_timestamp) AS node_comment_statistics_last_updated" .
	" FROM node INNER JOIN field_data_field_status2" . 
	" ON node.nid = field_data_field_status2.entity_id" .
	" AND (field_data_field_status2.entity_type = 'node' AND field_data_field_status2.deleted = '0')" .
	" INNER JOIN node_comment_statistics ON node.nid = node_comment_statistics.nid" .
	" WHERE (( (node.status = '1') AND (node.type IN  ('confidential_recommendation')) AND (field_data_field_status2.field_status2_tid = '426') ))";
//print($query);
$con_count = 0;
$con_dur_total = 0;
$con_avg_total = 0;
$result = mysql_query($query);
//check if current input already exist
if (!$result) 
{
    $message  = 'Error: ' . mysql_error() . "\n";
	die($message);
}
elseif ($result)
{
	print "Confidential requests";
	print "<br>\n";
	print "<table border='1'>";
	print "<tr>";
	print "<td><b>Title</b></td>";
	print "<td><b>Node ID</b></td>";
	print "<td><b>Post date</b></td>";
	print "<td><b>Status</b></td>";
	print "<td><b>Last update</b></td>";
	print "<td><b>Duration(Days)</b></td>";
	print "</tr>";
	//print "Title\tNode ID\tPost date\tStatus\tLast update\tduration";
	//print "<br>\n";
	while (list($title, $nid, $post, $status, $last_update) = mysql_fetch_array($result))
	{
		print "<tr>";
		$post = date('Y-m-d', $post);
		$last_update = date('Y-m-d', $last_update);

                $diff = abs(strtotime($post) - strtotime($last_update));
                $years = floor($diff / (365*60*60*24));
                $months = floor(($diff - $years * 365*60*60*24) / (30*60*60*24));
                $duration = floor(($diff - $years * 365*60*60*24 - $months*30*60*60*24)/ (60*60*24));
		//print "$title, $nid, $post, $status, $last_update, $duration days";
		//print "<br>\n";
		print "<td>$title</td>";
		print "<td>$nid</td>";
		print "<td>$post</td>";
		print "<td>$status</td>";
		print "<td>$last_update</td>";
		print "<td>$duration</td>";
		$con_count++;
		$con_dur_total = $con_dur_total + $duration;
		print "</tr>";
	}
	print "</table>" ;
	
	$con_avg_total = $con_dur_total / $con_count;
	print "<br>\n";
	print "<b>Avg. duration to fulfilled a confidential recomendation request = $con_avg_total</b>";
	print "<br>\n";
}

if ($noncon_count && $con_count) {
	$avg_total = ($noncon_dur_total + $con_dur_total) / ($noncon_count + $con_count);
	print "<br>\n";
	print "<b>Avg. duration to fulfilled by both confidential and non-confidential recomendation requests = $avg_total</b>";
	print "<br>\n";
}

?>
<!-- close database connection? -->
<?php mysql_close(); ?>