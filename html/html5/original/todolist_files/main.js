var todoLists;

function toggleToDo(id, oStatus)
{
    var oStatus  = $(oStatus);
    var statusOn = oStatus.hasClass('on');
    var oToDo    = $('#todo-lists #' + id + ' .todo');

    if ( statusOn )
    {
        oStatus.removeClass('on').addClass('off');
        oToDo.removeClass('on').addClass('off');
        todoLists[id].status = 'off';
    }
    else
    {
        oStatus.removeClass('off').addClass('on');
        oToDo.removeClass('off').addClass('on');
        todoLists[id].status = 'on';
    }

    saveToDo();

    return false;
}

function saveToDo()
{
    localStorage.todoList = JSON.stringify(todoLists);
}

function deleteToDo(id)
{
    $('#todo-lists #' + id).slideUp(300);

    for ( var i in todoLists )
    {
        if ( i == id )
        {
            delete todoLists[i];
        }
    }

    saveToDo();
}

function addToDo(id, oToDo)
{
    return '<div id="'+id+'"><div class="todo '+oToDo.status+'"><input type="checkbox" class="ch" />'+oToDo.text+'</div><div class="controls"><span class="status '+oToDo.status+'" onclick="toggleToDo('+id+', this)"></span><span class="close" onclick="deleteToDo('+id+')"><img src="img/close.png" /></span></div></div>';
}

$(document).ready(function()
{
    try{Typekit.load();}catch(e){}
    
    if ( !localStorage.todoList )
    {
        localStorage.todoList = '[]';
    }

    eval('todoLists = ' + localStorage.todoList);

    for ( var id in todoLists )
    {
        if ( todoLists[id] )
        {
            $('#todo-lists').append(addToDo(id, todoLists[id]));
        }
    }

    $('.add').click(function()
    {
        var toDoText = $('#todo-list-text').val();

        if ( toDoText )
        {
            var id = todoLists.length;
            todoLists[id] = {text:toDoText, status:'on'};
            $('#todo-lists').append(addToDo(id, todoLists[id]));
            saveToDo();

            $('#todo-list-text').val('');
        }

        return false;
    });

    $('.delete').click(function()
    {
        $('.ch:checked').each(function()
        {
            var id = $(this).parent().parent().attr('id');
            deleteToDo(id);
        });

        return false;
    });
});