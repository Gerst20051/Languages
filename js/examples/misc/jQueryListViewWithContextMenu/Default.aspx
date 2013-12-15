<%@ Page Language="C#" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<script runat="server">

    protected void ButtonHiddenAddCustomer_Click(object sender, EventArgs e)
    {
        LiteralNewCustomer.Text = @"<b>The following would of been saved:</b><br>" +
            "Contact: " + TextBoxContactName.Text + "<br>Title: " +
            TextBoxContactTitle.Text + "<br>Country: " + TextBoxCountry.Text +
            "<br>Phone: " + TextBoxCountry.Text;                            
    }
</script>    
    
<html xmlns="http://www.w3.org/1999/xhtml">
<head id="Head1" runat="server">
<title>Context Menu in a ListView Using jQuery Demo</title>        
<link href="_assets/css/StyleSheet.css" rel="stylesheet" type="text/css" />
<link href="_assets/css/confirm.css" rel="stylesheet" type="text/css" />    
<link href="_assets/css/jquery.contextMenu.css" rel="stylesheet" type="text/css" />

<script src="_assets/js/jquery-1.2.6.min.js" type="text/javascript"></script>
<script src="_assets/js/jquery.simplemodal-1.1.1.js" type="text/javascript"></script>
<script src="_assets/js/jquery.contextMenu.js" type="text/javascript"></script>

<!-- IE 6 hacks -->
<!--[if lt IE 7]>
<link type='text/css' href='_assets/css/confirm_ie.css' rel='stylesheet' media='screen' />
<![endif]-->

<script type="text/javascript">
    $(document).ready(function() {

        $("#addNewCustomerInstructionsImg").click(function(ev) {
            toggleAddCustomerInstructions();
        });

        $("#addNewCustomerInstructionsLink").click(function(ev) {
            ev.preventDefault();
            toggleAddCustomerInstructions();
        });

        $("#addNewCustomerInstructionsClose").click(function(ev) {
            ev.preventDefault();
            toggleAddCustomerInstructions();
        });

        $("#customerResponse").fadeOut(5000);

        $(".customerRow").contextMenu({ menu: 'myMenu' }, function(action, el, pos) { contextMenuWork(action, el, pos); });
        
        $(".openmenu").contextMenu({ menu: 'myMenu', leftButton: true }, function(action, el, pos) { contextMenuWork(action, el.parent("tr"), pos); });
    });

        function contextMenuWork(action, el, pos) {

            switch (action) {
                case "delete":
                    {
                        var msg = "Delete " + $(el).find("#contactname").text() + "?";
                        $("#HiddenFieldRowId").val($(el).find("#customerid").text());
                        confirm(msg);
                        break;
                    }
                case "insert":
                    {
                        $("#TextBoxContactName").val("");
                        $("#TextBoxContactTitle").val("");
                        $("#TextBoxCountry").val("");
                        $("#TextBoxPhone").val("");

                        $("#addNewCustomer").modal({
                            close: true,
                            onOpen: modalOpenAddCustomer,
                            onClose: modalOnClose,
                            persist: true,
                            containerCss: ({ width: "500px", height: "275px", marginLeft: "-250px" })
                        });
                        break;
                    }

                case "edit":
                    {
                        alert(
	    						'Action: ' + action + '\n\n' +
	    						'Element ID: ' + $(el).attr('id') + '\n\n' +
	    						'X: ' + pos.x + '  Y: ' + pos.y + ' (relative to element)\n\n' +
	    						'X: ' + pos.docX + '  Y: ' + pos.docY + ' (relative to document)'
	    						);
                    }
            }
        }

    function confirm(message) {
        $("#confirm").modal({
            close: true,
            overlayId: 'confirmModalOverlay',
            containerId: 'confirmModalContainer',
            onClose: modalOnClose,
            onShow: function modalShow(dialog) {
                dialog.overlay.fadeIn('slow', function() {
                    dialog.container.fadeIn('fast', function() {
                        dialog.data.hide().slideDown('slow');
                    });
                });

                dialog.data.find(".confirmmessage").append(message);

                // Yes button clicked
                dialog.data.find("#ButtonYes").click(function(ev) {
                    ev.preventDefault();
                    $.modal.close();
                    alert('The customer with id ' + $("#HiddenFieldRowId").val() + ' would of been deleted.');
                    //$("#ButtonDeleteCustomer").click();
                });
            }
        })
    }

    function modalOpenAddCustomer(dialog) {
        dialog.overlay.fadeIn('fast', function() {
            dialog.container.fadeIn('fast', function() {
                dialog.data.hide().slideDown('slow');
            });
        });

        dialog.data.find(".modalheader span").html("Add New Customer");

        // if the user clicks "yes"
        dialog.data.find("#ButtonAddCustomer").click(function(ev) {
            ev.preventDefault();

            //Perfom validation
            if (Page_ClientValidate("addCustomer")) {
                $.modal.close();
                $("#ButtonHiddenAddCustomer").click();
            }
            
        });
    }

    function toggleAddCustomerInstructions() {
        $("#addNewCustomerFields").toggle();
        $("#addNewCustomerInstructions").toggle()
    }

   function modalOnClose(dialog) {
       dialog.data.fadeOut('slow', function() {
           dialog.container.slideUp('slow', function() {
               dialog.overlay.fadeOut('slow', function() {
                   $.modal.close(); // must call this to have SimpleModal                   
                   // re-insert the data correctly and
                   // clean up the dialog elements
               });
           });
       });
    }    
                                   
</script>
</head>
<body>
    <form runat="server">
    <h1>Context Menu in a ListView Using jQuery Demo</h1>        
        <b>Instructions:</b>  
        <ul>
            <li>
                Click the lightning icon to open the context menu. You can then click Add New, Edit or Delete.
            </li>
            <li>
                <u>Right</u> click one of the rows in the table to open the same context menu. You can then click Add New, Edit or Delete.
            </li>
        </ul> 
    <div style="padding-left:70px;">                       
        <asp:ObjectDataSource ID="odsCustomers" runat="server" SelectMethod="Select" DeleteMethod="Delete" TypeName="CustomersDataObject" />
        <asp:ListView runat="server" ID="ListViewCustomers" DataSourceID="odsCustomers">
            <LayoutTemplate>
                <table id="customerTable" class="yui-grid" cellspacing="0" border="0">
                    <tr>
                        <th>
                            ID
                        </th>
                        <th>
                            Contact Name
                        </th>
                        <th>
                            Contact Title
                        </th>
                        <th>
                            Country
                        </th>                                                                                  
                        <th>
                            Phone
                        </th> 
                        <th>
                            &nbsp;
                        </th>                                                                                                                                                                   
                    </tr>
                    <tr id="itemPlaceholder" runat="server" />
                </table>
            </LayoutTemplate>
            <ItemTemplate>
                <tr id="row" class='<%# (Container.DataItemIndex % 2 == 0 ? "row" : "altrow") + " customerRow" %>'>                                   
                    <td id="customerid"><%# Eval("customerid") %></td>
                    <td id="contactname"><%# Eval("contactname")%></td>
                    <td><%# Eval("contacttitle")%></td>
                    <td><%# Eval("country")%></td>
                    <td><%# Eval("phone") %></td>                    
                    <td class="openmenu">
                        <img src="_assets/img/lightning.png" alt="Context Menu" title="View Context Menu" style="cursor:pointer;" /> 
                    </td>
                </tr>
            </ItemTemplate>
        </asp:ListView>         
    </div>
    <div>
        <ul>
            <li>
                This demo includes a modal delete confirmation which I wrote about <a href='http://beckelman.net/post/2008/10/28/Modal-Delete-Confirmation-Version-Two-Using-jQuery-SimpleModal-Plugin-Demo.aspx'>here</a>.
            </li>
            <li>
                It also includes inserting data through the simple modal plugin which I wrote about <a href='http://beckelman.net/post/2008/10/30/Inserting-Content-Using-jQuery-SimpleModal-Plugin-Demo.aspx'>here</a>.
            </li>
            <li>
                Icons from <a href='http://www.pinvoke.com/'>http://www.pinvoke.com/</a>
            </li>
        </ul>
    </div>

<asp:HiddenField ID="HiddenFieldRowId" runat="server" />

<!--Delete Customer-->
<div id="confirm" style="display:none">                    
    <div class="confirmheader"><span>Confirm</span></div>
        <p class="confirmmessage"></p>
    <div class="confirmbuttons">            
        <button id="ButtonYes" class="yui-button">Yes</button>&nbsp;
        <button id="ButtonNo" class="yui-button modalClose">No</button>
    </div>

    <!-- Hidden command buttons that actually issue the delete -->
    <asp:Button ID="ButtonDeleteCustomer" runat="server" 
        CausesValidation="false" style="display:none" />
</div>   

<!--Insert Customer-->
<div id="addNewCustomer" style="display:none;">
<div class="modalheader"><span></span></div>
<div class="modalbody">
<div style="padding:10px;">
    <img id="addNewCustomerInstructionsImg" src="_assets/img/information.png" alt="Instructions" title="Click to View Instructions" />
    <a href="#" id="addNewCustomerInstructionsLink" title="Click to View Instructions" style="text-decoration:none;">Instructions</a>
</div>
<div id="addNewCustomerInstructions" style="display:none;padding:10px;">
    <p>
    Praesent euismod. Donec nulla augue, venenatis scelerisque, dapibus a, consequat at, 
    leo. Pellentesque libero lectus, tristique ac, consectetuer sit amet, imperdiet ut, 
    justo. Sed aliquam odio vitae tortor. Proin hendrerit tempus arcu. In hac habitasse 
    platea dictumst. Suspendisse potenti. Vivamus vitae massa adipiscing est lacinia sodales. 
    </p>
    <button id="addNewCustomerInstructionsClose" class="yui-button">Close</button>     
</div>
<div id="addNewCustomerFields">
 <table>
    <tr>
        <th>
            Contact Name:
        </th>
        <td>
            <asp:TextBox ID="TextBoxContactName" Width="250px" 
                MaxLength="50" runat="server">
            </asp:TextBox>
            <asp:RequiredFieldValidator ID="RequiredFieldValidatorTextBoxContactName" runat="server" 
                ErrorMessage="* Required" ControlToValidate="TextBoxContactName"
                SetFocusOnError="true" ValidationGroup="addCustomer">
            </asp:RequiredFieldValidator>
        </td>
    </tr>
    <tr>
        <th>
            Title:
        </th>
        <td>
            <asp:TextBox ID="TextBoxContactTitle" Width="250px" 
                MaxLength="50" runat="server">
            </asp:TextBox>
            <asp:RequiredFieldValidator ID="RequiredFieldValidatorTextBoxContactTitle" runat="server" 
                ErrorMessage="* Required" ControlToValidate="TextBoxContactTitle"
                SetFocusOnError="true" ValidationGroup="addCustomer">
            </asp:RequiredFieldValidator>
        </td>
    </tr>                   
    <tr>
        <th>
            Country:
        </th>
        <td>
            <asp:TextBox ID="TextBoxCountry" Width="250px" 
                MaxLength="50" runat="server">
            </asp:TextBox>
            <asp:RequiredFieldValidator ID="RequiredFieldValidatorTextBoxCountry" runat="server" 
                ErrorMessage="* Required" ControlToValidate="TextBoxCountry"
                SetFocusOnError="true" ValidationGroup="addCustomer">
            </asp:RequiredFieldValidator>
        </td>
    </tr>    
    <tr>
        <th>
            Phone:
        </th>
        <td>
            <asp:TextBox ID="TextBoxPhone" Width="250px" 
                MaxLength="50" runat="server">
            </asp:TextBox>
            <asp:RequiredFieldValidator ID="RequiredFieldValidator1" runat="server" 
                ErrorMessage="* Required" ControlToValidate="TextBoxPhone"
                SetFocusOnError="true" ValidationGroup="addCustomer">
            </asp:RequiredFieldValidator>
        </td>
    </tr>     
</table>       
<div class="modalbodysubmitbuttons">
    <button id="ButtonAddCustomer" class="yui-button" title="Save Address">Save</button>&nbsp;        
    <button id="ButtonCancelAddCustomer" class="yui-button modalClose">Cancel</button>
 <!-- Hidden command button that actually issues the insert -->
<asp:Button ID="ButtonHiddenAddCustomer" runat="server" CssClass="addCustomer"
    style="display:none" OnClick="ButtonHiddenAddCustomer_Click" />     
</div>    
</div>
</div>
</div>


<!-- Right Click Menu -->
<ul id="myMenu" class="contextMenu">
    <li class="insert"><a href="#insert">Add New</a></li>		
    <li class="edit"><a href="#edit">Edit</a></li>			        
    <li class="delete"><a href="#delete">Delete</a></li>			
</ul>

<div id="customerResponse" style="padding-top:20px;">
<asp:Literal ID="LiteralNewCustomer" runat="server"></asp:Literal> 
</div>        
</form>
</body>
</html>
