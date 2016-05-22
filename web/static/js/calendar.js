window.onload = function(){
    var calendar = document.getElementById("calendarTable");
     for (var i = 1; i < calendar.rows.length; i++) {
        for (var j = 0; j < calendar.rows[i].cells.length; j++) {
            calendar.rows[i].cells[j].addEventListener("click",function(e){
                tableText(e);
            });
        }
    }
}

function listEvent(event){
    event.stopPropagation();
}

var day;
function tableText(e){
    var daytemp = e.currentTarget.innerText;
    day = daytemp;
    if($("#time") != undefined)
    {
        $("#time").empty().remove();
    }
    if(day == "")
    {
        return;
    }
    newSelect = "<select id = \"time\" style=\"position: absolute; z-index : 1;\" multiple onclick = \"listEvent(event);\">";
    for(i = 0; i<24; ++i)
    {
        newSelect +="<option> "
        if(i<10)
        {
            newSelect += "0";
        }
        newSelect += i +":00 </option>";
    }
    newSelect +="</select>";
    $(e.currentTarget).append(newSelect);
}
function getCookie(cname) {
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for(var i = 0; i <ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0)==' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length,c.length);
        }
    }
    return "";
} 
function nextMonth()
{
   var year = document.getElementById("year").innerHTML;
   var month = document.getElementById("month").innerHTML;
   $.ajax({
      type: "POST",
      url: "",
      data: {'date': {"year" : year, "month" : month}, 'dir': 'next', 'csrfmiddlewaretoken' : getCookie('csrftoken')},
      //if u get data call makeCalendar that will... make calendar
      success: function(data){
         makeCalendar(data);        
      }
   });
}
function prevMonth()
{
   var year = document.getElementById("year").innerHTML;
   var month = document.getElementById("month").innerHTML;
   $.ajax({
      type: "POST",
      url: "",
      data: {'date': {"year" : year, "month" : month}, 'dir': 'prev', 'csrfmiddlewaretoken' : getCookie('csrftoken')},
      //if u get data call makeCalendar that will... make calendar
      success: function(data){
         makeCalendar(data);
      }
   });
}
function reserve()
{
   selected = $("#time option:selected").text();
   if (day == undefined || day == "")
   {
      alert("nie wybrano dnia!");
      return;
   }
   if (selected == "")
   {
       alert("nie wybrano godzin!");
       return;
   }
  var reserveJSON = { month : $("#month").text(), year : $("#year").text(), day: day, time : selected };
  $.ajax({
      method: "POST",
      url: "",
      data: {'reserve' : JSON.stringify(reserveJSON), 'csrfmiddlewaretoken' : getCookie('csrftoken')},
      async: true,
      success : function(msg){
          alert(msg);
      }
  });
}
function resign()
{
   
}
function makeCalendar(data)
{
   var newTable = "";
   newTable += "<caption>\
<button id = \"prevMonth\" onclick = \"prevMonth()\"> &lt;&lt;</button>\
<div id = \"month\" style = \"display : inline\">"+ data.month +"</div> <div id = \"year\" style = \"display : inline\">"+data.year+"</div>\
<button id = \"nexMonth\" onclick = \"nextMonth()\"> &gt;&gt;</button>\
</caption>";
   newTable +="<tr>";
      for( i = 0; i<7; ++i )
      {
         newTable +="<th>"+data.dayNames[i]+"</th>";
      }
   newTable == "</tr>";
   for(i = 0; i<data.weeks.length; ++i)
   {
      newTable +="<tr>\n";
      for(j = 0; j<data.weeks[i].length;++j)
      {
         newTable +="<td align=\"right\">"+ data.weeks[i][j]+"</td>\n";
      }
      newTable +="</tr>\n";
   }
   newTable;
   $("#calendarTable").html(newTable);
    var calendar = document.getElementById("calendarTable");
     for (var i = 1; i < calendar.rows.length; i++) {
        for (var j = 0; j < calendar.rows[i].cells.length; j++) {
            calendar.rows[i].cells[j].addEventListener("click",function(e){
                tableText(e);
            });
        }
    }
}
/*
             <table>
          <caption>
             <button id = "prevMonth" onclick = "prevMonth()"> &lt;&lt;</button>
             <div id = "month" style = "display : inline">{{month}}</div> <div id = "year" style = "display : inline">{{year}}</div>
             <button id = "nexMonth" onclick = "nextMonth()"> &gt;&gt;</button>
          </caption>
             <tr>
               {% for dayName in dayNames %}
                <th>{{dayName}}</th>
                {% endfor %}
             </tr>
             
                {% for week in weeks %}
             <tr>
                {% for day in week %}
                <td align="right">{{day}}</td>
                {% endfor %}
             </tr>
          {% endfor %}
        </table>
        */