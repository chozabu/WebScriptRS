return;

lightbulb = "http://t3.gstatic.com/images?q=tbn:ANd9GcRKQt_3ZpHSJmRNRnHl2cQU7WqI3Nzsv3hjVJHkYxLvwG_JEd-1bYLn-fUE1A";
function s4() {
  return Math.floor((1 + Math.random()) * 0x10000)
             .toString(16)
             .substring(1);
};
function GUID() {
  return s4() + s4()+s4();
}
window.conceptUpdate=function(event){
     if(event.which==13){
         var imageListB = $('.imageListHolder');
         findImagesForTerms(event.target.value, function (result) {
                imageListB.html(result);
           });
     }
};

function basicBoxHTML(){
	c=$('#box').clone();
	c.find('div').remove('div').find('div').empty('div');
	return c.html();
}
function googImgClicked (img){
	var ji = $(img);
	ji.addClass('selectedImg');
	console.log('clicked img');
	silh = ji.parent().parent().parent().parent().find('.selectedImageListHolder');
	silh.append(ji.clone());
	//console.dir(img);
}
function findImagesForTerms(terms, imagesFoundCalback) {
    $.getJSON("https://ajax.googleapis.com/ajax/services/search/images?v=1.0&rsz=8&callback=?", {
        q: terms,
        start: 0
    }, function (images) {
        var imageList = $('<ol class="imageResults"></ol>');
        for (image in images.responseData.results) {
            thisImage = images.responseData.results[image];
            $(imageList).append("<li class='imageList'><img onmousedown='googImgClicked(this)' src='" + thisImage.tbUrl + "' /></li>");
            //console.dir(thisImage);
        }
        imagesFoundCalback(imageList);
        console.dir(images.responseData);
    });
}
//findImagesForTerms('nucleus accumbens', function (result) {
//    console.log('got result:');
//    console.log($(result).html());
//});

function getUnselectedText(containerEl) {
    var sel, range, tempRange, before = "", after = "";
    if (typeof window.getSelection != "undefined") {
        sel = window.getSelection();
        if (sel.rangeCount) {
            range = sel.getRangeAt(0);
        } else {
            range = document.createRange();
            range.collapse(true);
        }
        tempRange = document.createRange();
        tempRange.selectNodeContents(containerEl);
        tempRange.setEnd(range.startContainer, range.startOffset);
        before = tempRange.toString();

        tempRange.selectNodeContents(containerEl);
        tempRange.setStart(range.endContainer, range.endOffset);
        after = tempRange.toString();

        tempRange.detach();
    } else if ( (sel = document.selection) && sel.type != "Control") {
        range = sel.createRange();
        tempRange = document.body.createTextRange();
        tempRange.moveToElementText(containerEl);
        tempRange.setEndPoint("EndToStart", range);
        before = tempRange.text;

        tempRange.moveToElementText(containerEl);
        tempRange.setEndPoint("StartToEnd", range);
        after = tempRange.text;
    }
    return {
        before: before.split(' ').slice(-5).join(' '),
        after: after.split(' ').slice(0,5).join(' ')
    };
}

function surroundSelection() {
    var span = document.createElement("span");
    jspan = $(span);
    jspan.addClass('selectedTerms');
	var id='vizencoderHolderID'+GUID();
    jspan.attr('id',id);
    if (window.getSelection) {
        console.log(getUnselectedText($('#box')[0]));
        var sel = window.getSelection();
        if (sel.rangeCount) {
            var range = sel.getRangeAt(0).cloneRange();
            stringy = range.toString();
            range.surroundContents(span);
            sel.removeAllRanges();
            //sel.addRange(range);
            //range.removeAllRanges();
            jspan.append("<div class='vizencoderHolder'></div>");
            var vh = $(span).find('.vizencoderHolder'); //TODO: use ID instead of just class
            vh.append("<span class='concept'><img src='"+lightbulb+"' style='max-width:16px'/>Concept<input type='text' class='conceptBox' onkeypress='window.conceptUpdate(event)' value='"+stringy+"'></input></span>");
            vh.append("<span class='drawSpec'>✍Visual ideas<input type='text'></input></span>");
            vh.append("<span class='selectedImageListHolder'></span>");
            vh.append("<span class='imageListHolder'></span>");
            var imageList = vh.find('.imageListHolder');
            findImagesForTerms(stringy, function (result) {
                imageList.html(result);
            });

        }
    }
}


var text = document.getElementById('text')
