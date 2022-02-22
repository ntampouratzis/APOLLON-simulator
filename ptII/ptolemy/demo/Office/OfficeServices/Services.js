/** Accessor for the Services service. 
 *  This accessor queries the Services model via HTTP
 *  and produces the parsed JSON response on its output.
 *  It has embedded in it the IP address of the host running the Services model.
 *  The Services model, when it executes, modifies that IP address to conform
 *  with the IP address of whatever server is running the Services model
 *  and then publishes this accessor, modified with the correct IP address,
 *  to a KeyValueStoreServer running on the same host.
 *  
 *  On startup, this accessor sends null to the data output to
 *  signal anything downstream that it is starting up.
 *
 *  @accessor Services
 *  @author Edward A. Lee (eal@eecs.berkeley.edu)
 *  @input control A trigger, which can be anything, that causes this accessor
 *   to query for the status of services.
 *  @output data An object reporting the status of services.
 *  @output schema Not used by this accessor
 *  @version $$Id$$
 */
// Stop extra messages from jslint.  Note that there should be no
// space between the / and the * and global.
/*globals console, error, exports, require */
/*jshint globalstrict: true*/
"use strict";

exports.setup = function() {
	this.implement('ControllableSensor');
	var REST = this.instantiate('REST', 'net/REST');
	REST.input('options', {
	    'value': '"http://128.32.47.81:8080/json"'
	});
    this.connect('control', REST, 'trigger');
    this.connect(REST, 'response', 'data');
};

exports.initialize = function() {
	// At initialize, send the schema;
	this.send('schema', schema);
	// Also send null data.
	this.send('data', null);
};
var schema = {
  "type": "object",
  "properties": {
    "trigger": {
      	"type": "string",
      	"title": "Any trigger at all",
      	"description": "Any trigger at all",
      	"value": "request service status"
    }
  }
};
