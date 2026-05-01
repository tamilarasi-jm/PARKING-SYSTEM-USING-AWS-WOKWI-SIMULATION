import json
import boto3

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('ParkingSpots')

def lambda_handler(event, context):

    print("FULL EVENT:", event)

    # Handle both formats
    if "body" in event:
        body = json.loads(event["body"])
    else:
        body = event

    event_type = body.get("event_type")
    spot_id = body.get("spot_id")

    print("Parsed:", event_type, spot_id)

    if event_type == "ENTRY":
        table.update_item(
            Key={"spot_id": spot_id},
            UpdateExpression="SET #s = :val",
            ExpressionAttributeNames={"#s": "status"},
            ExpressionAttributeValues={":val": "occupied"}
        )

    elif event_type == "EXIT":
        table.update_item(
            Key={"spot_id": spot_id},
            UpdateExpression="SET #s = :val",
            ExpressionAttributeNames={"#s": "status"},
            ExpressionAttributeValues={":val": "available"}
        )

    return {
        "statusCode": 200,
        "body": json.dumps({"message": "updated"})
    }
