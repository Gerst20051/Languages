/*
    toc.js
    TechPubPerlModules
    
    Copyright (c) 2009 Apple Inc. All Rights Reserved.

    // This creates an expanding TOC based on the structure of the HTML.
    // 
    // For this to work, you need to identify various things in the TOC by 
    // adding a toc attribute to elements.  The possible values for a toc 
    // attribute are:
    //
    //  section: Contains the disclosure triangle, chapter link, and 
    //           the collapsible area that has sub-links for the chapter.
    //           Typically this is a div, though you can make it any element.
    //
    //  collapsible: Contains sub-links for a chapter, and is the portion to
    //           expand and collapse. Typically this is a span, though you
    //           can make it any element.
    //
    //  section_link: A link that should collapse/expand the section it is a part of.
    //           The section is closed/opened on clicking this link or the triangle.
    //           Typically both the link for the triangle and the link for the text
    //           next to the triangle are set to this.
    //
    // In addition to the toc attributes, if you want a disclosure triangle you 
    // need to provide img elements with 'open' and 'closed' attributes indicating
    // what image to display in each case.
    //
    // Here is the typical structure of a toc section (additional formatting/table tags
    // can be placed throughout as needed):
    //
    // <div toc="section">
    //      ...<a toc="section_link" href="content.html"><img src="open.gif" open="open.gif" closed="closed.gif"></a>...
    //      ...<a toc="section_link" href="content.html">Bundles</a>...
    //      ...<span toc="collapsible">collapsible content</span>...
    // </div>

*/


// JSLint Configuration
/*jslint plusplus: false, browser: true */
/*global window location escape localStorage navigator*/

// page.js
/*global ContentFrameName, delete_toc_cookie, get_toc_cookie, set_toc_cookie */


var from_toc_frame = 0;     // Track whether request originated from TOC frame to deal with a Mac IE bug.
var isJavaScriptTOC = 0;    // If this looks like a JavaScript TOC this is set to 1.
var ignore_page_load = 0;   // If user clicks TOC link to load page, don't do display stuff to TOC that's done for next/previous buttons, etc.
var lastSelectedItem;       // So we can change the formatting back to normal.
var lastSelectedColor;      // The color it used to be.
var lastSelectedWeight;     // The font weight it used to be

function set_xcode_state() {
    var xcodeVersion,
        versionRegexp = new RegExp("Xcode\/([0-9.]+)"),
        xcodeVersionArray = versionRegexp.exec(navigator.appVersion);
        
    if (xcodeVersionArray) {
        xcodeVersion = parseFloat(xcodeVersionArray[1]);
    }
        
    if (xcodeVersion && xcodeVersion < 1000.0) {
        document.getElementById("base_target").target = ContentFrameName();
    }
}

function toc_section_parent(element) {
    // Find the first parent with a toc attribute of "section".
    var toc_parent = element.parentNode;
    
    while (toc_parent) {
        if (toc_parent.tagName === 'body') {
            // Have to stop checking here, because the HTML element above this doesn't have a getAttribute() function
            // and will crash the next check.
            return;
        }
        
        if (toc_parent.getAttribute('toc') === 'section') {
            return toc_parent;
        }
        
        toc_parent = toc_parent.parentNode;
    }
}

function isNetscape() {
    // WebKit shows up as Netscape but doesn't have the same problems, so exclude that case.
    return false;
}

function isMacIE() {
    // no longer supported
    return false;
}

function collapsible_child(parent) {
    // Get the first descendant with toc="collapsible".
    // To get all descendants, ask for *.
    var children = parent.getElementsByTagName("*"),
        child, i;

    if (children.length) {
        for (i = 0; i < children.length; i++) {
            child = children[i];
            if (child.getAttribute("toc") === "collapsible") {
                return child;
            }
        }
    }
}


function disclosure_image(div) {
    var images = div.getElementsByTagName("IMG"),
        i,
        image,
        open;

    if (images.length) {
        for (i = 0; i < images.length; i++) {
            image = images[i];
            open = image.getAttribute("open");
            if (open) {
                return image;
            }
        }
    }
}

function collapse(div) {
    if (isNetscape()) {
        // Netscape can't deal with setting display to none after initial page load,
        // so skip expanding/collapsing.
        return;
    }
    
    var i,
        current_div,
        child,
        image,
        divs = document.getElementsByTagName("DIV");

    if (divs.length) {
        for (i = 0; i < divs.length; i++) {
            current_div = divs[i];
            if (div === current_div) { 
                delete_toc_cookie("chapter_" + i);
            }
        }
    }
    
    if (div.className !== "jtoc_closed") {
        div.className = "jtoc_closed";
        
        child = collapsible_child(div);
        if (child) {
            child.style.display = "none";
        }
        
        // Skip image changing for Mac IE since some frame-related bug
        // messes that up when things originated from another frame.
        if (from_toc_frame || !isMacIE()) {
            image = disclosure_image(div);
            if (image) {
                image.src = image.getAttribute("closed");
            }
        }
    }
}

function next_div_sibling(sibling) {
    var nextS = sibling.nextSibling,
        sibling2;

    if (nextS) {
        sibling2 = nextS.nextSibling;
        return sibling2;
    }
    
}

function prev_div_sibling(sibling) {
    var prevS = sibling.previousSibling,
        sibling2;

    if (prevS) {
        sibling2 = prevS.previousSibling;
        return sibling2;
    }
}

function expand(div) {
    var child,
        image,
        i,
        divs,
        current_div,
        prev,
        next_sib,
        gets_top,
        gets_bottom;

    if (isNetscape()) {
        // Netscape can't deal with setting display to none after initial page load,
        // so skip expanding/collapsing.
        return;
    }
    
    if (div.className.indexOf("jtoc_open") === -1) {
        
        div.className = "jtoc_open";
                
        child = collapsible_child(div);
        if (child) { 
            child.style.display = "";
        }
        
        // Skip image changing for Mac IE since some frame-related bug
        // messes that up when things originated from another frame.
        if (from_toc_frame || !isMacIE()) {
            image = disclosure_image(div);
            if (image) { 
                image.src = image.getAttribute("open");
            }
        }
    }
    
    // Check each section to set the borders according to whether the previous/next section is open and such.
    divs = document.getElementsByTagName("DIV");
    if (divs.length) {
        
        for (i = 0; i < divs.length; i++) {
            
            current_div = divs[i];
            
            if ((current_div.className && current_div.className.indexOf("jtoc_open") !== -1)) {
                prev = prev_div_sibling(current_div);
                next_sib = next_div_sibling(current_div);
                
                gets_top = 0;
                gets_bottom = 0;
                                
                if (prev && prev.className && prev.className.indexOf("jtoc_open_bottom_line") === -1 && prev.className.indexOf("jtoc_open_both_lines") === -1) {
                    gets_top = 1;
                }
                
                if (next_sib && next_sib.className && next_sib.className.indexOf("jtoc_open_top_line") === -1 && next_sib.className.indexOf("jtoc_open_both_lines") === -1) {
                    gets_bottom = 1;
                }
                
                if (i === divs.length - 1) {
                    gets_bottom = 1;
                }
                
                // If I'm the first div
                if (prev && !prev.className) {
                    gets_top = 1;
                }
                
                // If I'm the last div
                if (next_sib && !next_sib.className) {
                    gets_bottom = 1;
                }
                
                if (gets_top === 1 && gets_bottom === 1) {
                    current_div.className = "jtoc_open_both_lines";
                } else if (gets_top === 1) {
                    current_div.className = "jtoc_open_top_line";
                } else if (gets_bottom === 1) {
                    current_div.className = "jtoc_open_bottom_line";
                } else {
                    current_div.className = "jtoc_open";
                }
                   
                set_toc_cookie("chapter_" + i, "open");
            }
        }
    }   
}

function expand_only(div) {
    // Collapse all but the specified section.
    // If div is null, everything is collapsed.
    var divs = document.getElementsByTagName("DIV"),
        i,
        current;
    if (divs.length) {
        for (i = 0; i < divs.length; i++) {
            current = divs[i];
 
            if (div && current === div || get_toc_cookie("chapter_" + i)) {
                expand(current);
            } else {
                collapse(current);
            }
        }
    }
}

function toggle(div) {
    // Toggle the div's disclosure status.
    if (div.className) {
        if (div.className.indexOf("jtoc_open") !== -1) {
            collapse(div);
        } else if (div.className === "jtoc_closed") {
        
            // if topics toc, expand_only
            if (this.is_topics_toc(div)) {
                expand_only(div);
            } else {
                expand(div);
            }
        }
    }
}

function link_action() {
    // Called by a link when clicked.
    // If this is a regular link, open the section if it's closed.
    // If it contains a disclosure triangle, toggle the section.
    
    // Since the page load is being driven by the user clicking on the TOC,
    // we don't want to go through the display stuff we do if the page is
    // being loaded by clicking on links in the content.
    ignore_page_load = 1;
    
    var div = toc_section_parent(this);
    
    if (div && div.className) {
        from_toc_frame = 1;
        // If it's a section_link, toggle the state of this section.
        if (this.getAttribute('toc') === 'section_link' || this.parentNode.getAttribute('toc') === 'section_link') {
            toggle(div);
        }
        
        from_toc_frame = 0; 
    }
}

function set_initial_state() {    
    var i,
        divs = document.getElementsByTagName("DIV");
        
    // Set action for all links, expand first chapter and collapse the rest.
    for (i = 0; i < document.links.length; i++) {
        document.links[i].onmousedown = link_action;
    }

    if (divs.length) {
        isJavaScriptTOC = 1;
        from_toc_frame = 1;
                
        for (i = 0; i < divs.length; i++) {
            if (get_toc_cookie("chapter_" + i)) {
                expand(divs[i]);
            } else {
                collapse(divs[i]);
            }
            
        }
        
        from_toc_frame = 0;
    }    
}

function path_suffix(path) {
    // Returns last two path segments as a string: "leaf_dir/filename.html".
    var leaf = "",
        parent = "",
        length,
        path_array = path.split('#');// First split on # to get rid of any anchor at end of path.
    
    path = path_array[0];
        
    // Now split apart the path.
    // 3379110: Was using array.pop() function, but it failed in Mac IE.
    path_array = path.split('/');
    length = path_array.length;
    
    if (length) { 
        leaf = path_array[length - 1];
    }
    if (length > 1) { 
        parent = path_array[length - 2];
    }
    if (parent && leaf) {
        return parent + '/' + leaf;
    } else { 
        return leaf;
    }
}

function last_path_component(path) {
    // Returns the last path segments as a string: "filename.html" from "leaf_dir/filename.html".
    var leaf = "",
        parent = "",
        path_array = path.split('#'), // First split on # to get rid of any anchor at end of path.
        length;
        
    path = path_array[0];
        
    // Now split apart the path.
    // 3379110: Was using array.pop() function, but it failed in Mac IE.
    path_array = path.split('/');
    length = path_array.length;
    
    if (length) { 
        leaf = path_array[length - 1];
    }
    return leaf;

}

function is_location_page_unique_in_toc(page_location) {
    // Returns whether the page location matches exactly one entry in the toc.
    // Since match is based on filename, a single file may have many locations in the toc, esp for reference.
    var page_suffix = path_suffix(page_location.pathname),
        all_links = document.links,
        match_count = 0,
        i,
        anchor,
        anchor_suffix;
    
    for (i = 0; i < all_links.length; i++) {
        anchor = all_links[i];
        anchor_suffix = path_suffix(anchor.getAttribute("HREF"));
        
        if (page_suffix === anchor_suffix) {
            match_count++;
        }
        if (match_count > 1) {
            return 0;
        }
    }
        
    if (match_count === 1) {
        return 1;
    } else {
        // There was no match. This sometimes happens if the file is at the root level of the document, which is the case with
        // some revision histories and intros. Try matching on just filename rather than directory/filename.
        page_suffix = last_path_component(page_location.pathname);
        match_count = 0;
        
        for (i = 0; i < all_links.length; i++) {
            anchor = all_links[i];
            anchor_suffix = last_path_component(anchor.getAttribute("HREF"));
            
            if (page_suffix === anchor_suffix) {
                match_count++;
            }
            if (match_count > 1) {
                return 0;
            }
        }
    }
    if (match_count === 1) {
        return 1;
    } else {
        // Truely no match
        return 0;
    }
}

function page_location_matches(page_location) {
    // Returns the first location the page matches.
    // Note that a page may match many locations, so is_location_page_unique_in_toc() can be called first to check uniqueness.
    var page_suffix = path_suffix(page_location.pathname),
        all_links = document.links,
        anchor,
        i,
        anchor_suffix;
    
    for (i = 0; i < all_links.length; i++) {
        anchor = all_links[i];
        anchor_suffix = path_suffix(anchor.getAttribute("HREF"));
        
        if (page_suffix === anchor_suffix) {
            return anchor;
        }
    }
    
    // No match. Try again at root level
    page_suffix = last_path_component(page_location.pathname);
    
    for (i = 0; i < all_links.length; i++) {
        anchor = all_links[i];
        anchor_suffix = last_path_component(anchor.getAttribute("HREF"));
        
        if (page_suffix === anchor_suffix) {
            return anchor;
        }
    }
}

function page_loaded(page_location) {
    // Called by a page on loading, so we can track what page is displayed.
    // If there is a link that points to the loaded page, make sure that TOC
    // section is disclosed and turn that link black.
    if (isJavaScriptTOC) {
        var page_suffix = path_suffix(page_location.pathname),
            all_links = document.links,
            anchor = page_location_matches(page_location),
            parent;

        if (anchor) {
            if (lastSelectedItem) {
                lastSelectedItem.style.color = lastSelectedColor;
                lastSelectedItem.style.fontWeight = lastSelectedWeight;
            }

            lastSelectedItem = anchor;
            lastSelectedColor = anchor.style.color;
            lastSelectedWeight = anchor.style.fontWeight;

            if (is_location_page_unique_in_toc(page_location)) {
                anchor.style.color = "black";
                anchor.style.fontWeight = "bold";
            }

            // If this page load didn't come from the TOC,
            // get the parent section, expand it and close others.
            if (ignore_page_load) {
                ignore_page_load = 0;
            } else {
                parent = toc_section_parent(anchor);
                expand_only(parent);
            }
        }
    }
}

function initialize_toc() {
        
    // Workaround for Xcode, for now
    set_xcode_state();
    
     // Called on page load to setup the page.    
    set_initial_state();
    
    // Page is now loaded
    page_loaded(parent.document.location);
}

function is_topics_toc(element) {
    // Return if my parent div's id attribute is "topics"
    var toc_parent = element.parentNode;
    
    while (toc_parent && toc_parent.getAttribute("id") && toc_parent.getAttribute("id") !== "topics") {

        toc_parent = toc_parent.parentNode;
    }
    
    if (toc_parent.getAttribute("id") === "topics") {
        return toc_parent;
    }
}

function disclosure_triangle() {
    // The mapping table sets disclosure triangles to call this.
    var parent = toc_section_parent(this);
    if (parent) { 
        toggle(parent);
    }
    return 0;
}
