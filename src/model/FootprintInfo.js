import mongoose from "mongoose";

const Schema = mongoose.Schema;

const FootPrintSchema = new Schema({
    time: { type: String},
    open: { type: Number, default: 0},
    high: { type: Number, default: 0 },
    low: { type: Number, default: 0 },
    close: { type: Number, default: 0 },
    priceLevels: [{ type: Object }],
});

export default mongoose.model("FootPrint", FootPrintSchema);