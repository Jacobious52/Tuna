var passed = 0;
var failed = 0;

function setSize(event) {
    var testPanel = document.getElementById("testpanel");
    var varPanel = document.getElementById("varpanel");

    var h = (window.innerHeight - 100).toString() + "px";

    testPanel.style.height = h;
    varPanel.style.height = h;
}

window.onresize = setSize;
document.addEventListener("DOMContentLoaded", setSize);

function createTestAlert(json) {
    var ul = document.getElementById("tests");

    var li = document.createElement("div");

    if (json.Result == "PASS") {
        li.setAttribute("class", "alert alert-success");
        passed++;
    } else {
        li.setAttribute("class", "alert alert-danger");
        failed++;
    }

    var strong = document.createElement("STRONG");
    strong.appendChild(document.createTextNode(json.Result + ": "));
    li.appendChild(strong);
    li.appendChild(document.createTextNode(json.Description));

    if (json.Result == "FAIL") {
        var p1 = document.createElement("PRE");
        p1.appendChild(document.createTextNode("Expected: " + json.Expected + "\nGot: " + json.Got));
        li.appendChild(p1);
    }

    ul.appendChild(li);

    var progPassed = document.getElementById("passed");
    var progFail = document.getElementById("failed");

    var percent = (passed)/(passed + failed) * 100;

    progPassed.style.width = percent.toString() + "%";
    progFail.style.width = (100 - percent).toString() + "%";
}

function createLogAlert(json) {
    var ul = document.getElementById("log");

    var li = document.createElement("li");
    switch (json.Level) {
        case "INFO":
            li.setAttribute("class", "list-group-item list-group-item-info");
            break;
        case "WARN":
            li.setAttribute("class", "list-group-item list-group-item-warning");
            break;
        default:
            log.error("log level unknown. " + json.level);
    }

    var strong = document.createElement("STRONG");
    strong.appendChild(document.createTextNode(json.Level + ": "));
    li.appendChild(strong);

    li.appendChild(document.createTextNode(json.Message));

    ul.appendChild(li);
}

function init() {

    var source = new EventSource("/events")

    source.onmessage = function (e) {

        var json = JSON.parse(e.data)
        console.log(JSON.stringify(json));

        switch (json.Command) {
            case "TEST":
                createTestAlert(json);
                break;
            case "LOG":
                createLogAlert(json);
                break;
            default:
                console.error("unknown command. " + json.Command);
        }
    }

    source.onerror = function (e) {
        console.log("readyState = " + e.currentTarget.readyState)
    }
}
init();
