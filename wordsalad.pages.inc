<?php

/**
 * @file
 * Page callback file for the Writing365 module.
 *
 * 2-10-2016 Added page callback for Wordsalad journal user data - Lines: 94-152
 */

/**
 * Menu callback; displays a Drupal page containing recent journal entries of a given user.
 */

/**
 * Menu callback; displays a Drupal page containing recent journal entries of all users.
 */

function wordsalad_page_info_last() {
    global $user;
    $build = array();
    drupal_set_title($title = t("Write 365 Journal WordSalad Summary"), PASS_THROUGH);

    $build = "<p>WordSalad is a text analysis service designed to analyze the authenticity of journal entries. WordSalad computes a probability that a journal entry was written by a person rather then fabricated using other means such as text-to-speech or random key strokes. Note: Only journal entries after January 7th, 2016 are included in this view.</p>";
    $build .= "<p><i class=\" fa fa-exclamation-triangle fa-2x\" style=\"color:#f08080;\" title=\"Exclamation Triangle\"></i>&nbsp;&nbsp;A WordSalad value indicating that the journal entry is <u>probably not</u> structured English.<br />";
    $build .= "<i class=\" fa fa-check fa-2x\" style=\"color:#90ee90;\" title=\"Check Mark\"></i>&nbsp;&nbsp;A WordSalad value indicating that the journal entry is <u>probably</u> structured English.</p>";


    # configure the table header columns
    $header = array(
        //array('data' => 'Node ID', 'field' => 'nid'),
        array('data' => 'Title', 'field' => 'title', 'sort' => 'ASC'),
        array('data' => 'Created', 'field' => 'created',       'sort' => 'ASC'),
        array('data' => 'Status', 'field' => 'is_wordsalad'),
        array('data' => 'Wordcount', 'field' => 'wordcount'),
    );



    $result = db_select('node', 'n')->extend('PagerDefault')->limit(20);

    $result->fields('n', array('nid','title','type','created'))
        ->fields('w',array('is_wordsalad','percent','score'))
        ->fields('wc',array('wordcount','timewriting','wordsperminute'))
        ->condition('type', 'writing365')
        ->condition('status', 1)
        ->addTag('node_access');

    $result->join('wordsalad_score','w', 'w.nid = n.nid');
    $result->join('W365_JournalNodeData','wc', 'wc.nid = n.nid');

    $result->extend('TableSort')
        ->orderByHeader($header); //then use an extender to create the sorted table

    $results = $result->execute();


    $rows = array();

    foreach ($results as $row) {
        if($row->is_wordsalad){
            $wordsalad = '<i class=" fa fa-exclamation-triangle fa-2x" style="color:#f08080;" title="Exclamation Triangle"></i>';
        }
        else {
            $wordsalad = '<i class=" fa fa-check fa-2x" style="color:#90ee90;" title="Check Mark"></i>';
        }
        $rows[] = array(
            //"nid"  =>  $row->nid,
            "title" => "<a href='/node/".$row->nid."''/>".$row->title."</a>",
            "created" => date('F j, Y, g:i a', $row->created),
            "is_wordsalad" => $wordsalad,
            "wordcount" => $row->wordcount,
        );
    }

    $build .= theme('table', array('header' => $header, 'rows' => $rows ));

    # add the pager
    $build .= theme('pager');

    return $build;
}

/**
 * Menu callback; displays a Drupal page containing recent journal entries, wordcount and wordsalad of user.
 */

function wordsalad_page_info($account) {
    global $user;
    $uid = get_user_id();
    $build = array();
    drupal_set_title($title = t("@name's Write 365 Journal Info", array('@name' => format_username($account))), PASS_THROUGH);

    $build = "<p>WordSalad is a text analysis service designed to analyze the authenticity of journal entries. WordSalad computes a probability that a journal entry was written by a person rather then fabricated using other means such as text-to-speech or random key strokes. Note: Only journal entries after January 7th, 2016 are included in this view.</p>";
    $build .= "<p><i class=\" fa fa-exclamation-triangle fa-2x\" style=\"color:#f08080;\" title=\"Exclamation Triangle\"></i>&nbsp;&nbsp;A WordSalad value indicating that the journal entry is <u>probably not</u> structured English.<br />";
    $build .= "<i class=\" fa fa-check fa-2x\" style=\"color:#90ee90;\" title=\"Check Mark\"></i>&nbsp;&nbsp;A WordSalad value indicating that the journal entry is <u>probably</u> structured English.</p>";


    # configure the table header columns
    $header = array(
        //array('data' => 'Node ID', 'field' => 'nid'),
        array('data' => 'Title', 'field' => 'title', 'sort' => 'ASC'),
        array('data' => 'Created', 'field' => 'created',       'sort' => 'ASC'),
        array('data' => 'Status', 'field' => 'is_wordsalad'),
        array('data' => 'Wordcount', 'field' => 'wordcount'),

    );

    $result = db_select('node', 'n')->extend('PagerDefault')->limit(20);

    $result->fields('n', array('nid','title','type','created'))
        ->fields('w',array('is_wordsalad','percent','score'))
        ->fields('wc',array('wordcount','timewriting','wordsperminute'))
        ->condition('type', 'writing365')
        ->condition('status', 1)
        ->condition('uid', $account->uid)
        ->addTag('node_access');
    
    $result->join('wordsalad_score','w', 'w.nid = n.nid');
    $result->join('W365_JournalNodeData','wc', 'wc.nid = n.nid');

    $result->extend('TableSort')
        ->orderByHeader($header); //then use an extender to create the sorted table


    $results = $result->execute();


    $rows = array();

    foreach ($results as $row) {
        if($row->is_wordsalad){
            $wordsalad = '<i class=" fa fa-exclamation-triangle fa-2x" style="color:#f08080;" title="Exclamation Triangle"></i>';
        }
        else {
            $wordsalad = '<i class=" fa fa-check fa-2x" style="color:#90ee90;" title="Check Mark"></i>';
        }
        $rows[] = array(
            //"nid"  =>  $row->nid,
            "title" => "<a href='/node/".$row->nid."''/>".$row->title."</a>",
            "created" => date('F j, Y, g:i a', $row->created),
            "is_wordsalad" => $wordsalad,
            "wordcount" => $row->wordcount,
        );
    }

    $build .= theme('table', array('header' => $header, 'rows' => $rows ));

    # add the pager
    $build .= theme('pager');

    return $build;

}