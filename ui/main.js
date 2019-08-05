/* main.js: handle file upload and graph/statistics display in the UI. */
let htData = null;
const selectors = {};

window.onload = function() {
    const statsTable = document.querySelector("#stats-table");
    const graph = document.querySelector("#graph");
    const fileUpload = document.getElementById("fileUpload");
    fileUpload.addEventListener("change", uploadHandler, false);
    selectors["statsTable"] = statsTable;
    selectors["graph"] = graph;

    /* render views once when page is loaded */
    updateView(selectors);
}

/* Data controller. */
function uploadHandler(event) {
    const file = event.target.files;
    const reader = new FileReader();

    reader.onload = parseJSON;
    reader.readAsText(file[0]);
}

function parseJSON(event) {
    let content = event.target.result;
    htData = JSON.parse(content);
    console.log(htData);
    updateView(selectors);
}

/* Render views. */
function updateView(selectors) {
    if (isQueryVarSet("statistics"))
        showStatisticsView(selectors);
    else if (isQueryVarSet("graph"))
        showGraphView(selectors);
    else
        showDefaultView(selectors);
}

function showStatisticsView(selectors) {
    if (htData !== null) {
        selectors.statsTable.style.display = "inline-table";
        const elapsedPerRun = Math.round((htData.time_elapsed / htData.total_runs) / 10) / 100;
        const elapsedTotal = Math.round(htData.time_elapsed / 10) / 100;

        document.querySelector("#td-1").innerHTML = htData.longest_list;
        document.querySelector("#td-2").innerHTML = htData.shortest_list;
        document.querySelector("#td-3").innerHTML = htData.number_of_zero_length_lists;
        document.querySelector("#td-4").innerHTML = htData.max_hashes_per_run;
        document.querySelector("#td-5").innerHTML = htData.actual_hashes_per_run;
        document.querySelector("#td-6").innerHTML = htData.table_size;
        document.querySelector("#td-7").innerHTML = elapsedTotal + " secs";
        document.querySelector("#td-8").innerHTML = elapsedPerRun + " secs";
        document.querySelector("#td-9").innerHTML = htData.total_runs;
    }
}

function showGraphView(selectors) {
    if (htData !== null) {
        selectors.graph.style.display = "inline-table";
        const ctx = selectors.graph.getContext('2d');
        console.log(htData.list_table);
        let labels = [];
        for (let i = 0; i < htData.list_table.length; i++)
            labels.push(i);
        const chart = new Chart(ctx, {
            type: "line",
            data: {
                labels: labels,
                datasets: [{
                    data: htData.list_table,
                    label: "List Length"
                }]
            },
            options: {
                scales: {
                    yAxes: [{
                        ticks: {
                            beginAtZero: true
                        }
                    }]
                }
            }
        });
    }
}

function showDefaultView(selectors) {
    showStatisticsView(selectors);
}

/* Helper functions. */
function isQueryVarSet(wanted) {
    let query = window.location.search.substring(1);
    let vars = query.split("&");

    for (let i = 0; i < vars.length; i++) {
        if (vars[i] === wanted)
            return true;
    }

    return false;
}
